#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <MPU6050.h>

// 🔐 WiFi Credentials
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// 🌐 Server (friend laptop IP)
String serverName = "http://YOUR_LAPTOP_IP:5000/api/potholes";

// 📡 GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// 📊 MPU
MPU6050 mpu;

// 🔥 Baseline variables
float baseline = 1.0;
int samples = 100;

// 🎯 Deviation thresholds
float small_th = 0.5;
float medium_th = 1.0;
float big_th = 2.0;

// ==========================
// 🔧 BASELINE CALIBRATION
// ==========================
void calibrateBaseline() {
  Serial.println("🔄 Calibrating... Keep device still");

  float sum = 0;

  for (int i = 0; i < samples; i++) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    float Az = az / 16384.0;
    sum += Az;

    delay(20);
  }

  baseline = sum / samples;

  Serial.print("✅ Baseline (g): ");
  Serial.println(baseline);
}

// ==========================
// 🔧 SEVERITY FUNCTION
// ==========================
int getSeverity(float Az) {
  float deviation = abs(Az - baseline);

  if (deviation > big_th) {
    Serial.println("🔴 BIG POTHOLE");
    return 3;
  }
  else if (deviation > medium_th) {
    Serial.println("🟡 MEDIUM POTHOLE");
    return 2;
  }
  else if (deviation > small_th) {
    Serial.println("🟢 SMALL POTHOLE");
    return 1;
  }
  else {
    return 0;
  }
}

// ==========================
// 🔧 SETUP
// ==========================
void setup() {
  Serial.begin(115200);

  // 📡 GPS Init
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  // 📊 MPU Init
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 Connected ✅");
  } else {
    Serial.println("MPU6050 Failed ❌");
  }

  // 🔥 Baseline calibration
  calibrateBaseline();

  // 🌐 WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected ✅");
}

// ==========================
// 🔁 LOOP
// ==========================
void loop() {

  // 📡 Read GPS
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // 📊 Read MPU
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float Az = az / 16384.0;
  float deviation = abs(Az - baseline);

  Serial.print("Az: ");
  Serial.print(Az);
  Serial.print(" | Deviation: ");
  Serial.println(deviation);

  // 🎯 Get severity
  int severity = getSeverity(Az);

  // 🚨 Detect pothole
  if (severity > 0 && gps.location.isValid()) {

    float lat = gps.location.lat();
    float lon = gps.location.lng();

    Serial.println("🚨 POTHOLE DETECTED!");
    Serial.print("Severity: ");
    Serial.println(severity);

    Serial.print("Lat: ");
    Serial.println(lat, 6);

    Serial.print("Lon: ");
    Serial.println(lon, 6);

    // 🌐 Send to server
    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{";
      jsonData += "\"latitude\":" + String(lat, 6) + ",";
      jsonData += "\"longitude\":" + String(lon, 6) + ",";
      jsonData += "\"severity\":" + String(severity);
      jsonData += "}";

      int response = http.POST(jsonData);

      Serial.print("HTTP Response: ");
      Serial.println(response);

      http.end();
    }
  }

  delay(800);
}