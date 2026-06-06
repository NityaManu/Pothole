# Pothole Detection and Mapping System using Bike Accelerometors
A real-time smart road monitoring system that uses bike-mounted accelerometer data and GPS coordinates to automatically detect, classify, and map potholes for improved road maintenance and rider safety.
## Overview
Road potholes are a major cause of vehicle damage, accidents, and inefficient transportation infrastructure. This project presents an IoT-based pothole detection and mapping system that automatically identifies road surface anomalies using accelerometer data collected from a moving vehicle. The system leverages an ESP32 microcontroller, MPU6050 sensor, and GSM/GPS communication to detect potholes, record their locations, and visualize them on a Flask-based web dashboard for real-time road condition monitoring.
## Features
1. Real-time pothole detection using MPU6050 accelerometer and gyroscope data
2. ESP32-based data acquisition and processing
3. GPS-enabled location tracking of detected potholes
4. GSM-based wireless data transmission
5. Flask-powered web dashboard for visualization and monitoring
6. Interactive map displaying pothole locations
7. Centralized storage and management of road condition data
8. Low-cost and scalable IoT architecture
## Hardware Components
1. ESP32 Development Board
2. MPU6050 Accelerometer and Gyroscope Sensor
3. GSM Neo-6m Module
## Software Stack
1. Embedded C
2. Python
3. Flask
4. HTML, CSS, JavaScript
5. Leaflet.js
## Working Principle
1. The MPU6050 continuously measures acceleration and vibration data from the vehicle.
2. ESP32 processes sensor readings and identifies sudden vertical acceleration spikes associated with potholes.
3. Upon detection, the GSM module acquires the geographical coordinates, and the detected pothole information and location data are transmitted via the GSM module.
4. Flask backend receives and stores the incoming data.
5. The web dashboard visualizes pothole locations on an interactive map for monitoring and analysis.
## Pothole Classification
Minor Pothole

Moderate Pothole

Severe Pothole

Classification is performed based on the threshold decided for the magnitude of acceleration changes detected by the MPU6050 sensor.
## Results
1. Successfully detected road surface irregularities in real time.
2. Accurately mapped pothole locations using GPS coordinates.
3. Enabled remote monitoring through GSM communication.
Visualized pothole hotspots on a Flask-based web dashboard.
Demonstrated a low-cost solution for smart road infrastructure monitoring.
