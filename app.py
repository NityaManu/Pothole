from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import sqlite3
from datetime import datetime
import math

app = Flask(__name__)
CORS(app)

DB_NAME = "potholes.db"

# ✅ Initialize DB
def init_db():
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS potholes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            latitude REAL,
            longitude REAL,
            severity REAL,
            timestamp TEXT
        )
    ''')
    conn.commit()
    conn.close()

init_db()

# ✅ Distance check
def haversine(lat1, lon1, lat2, lon2):
    R = 6371000
    phi1, phi2 = math.radians(lat1), math.radians(lat2)
    dphi = math.radians(lat2 - lat1)
    dlambda = math.radians(lon2 - lon1)

    a = math.sin(dphi/2)**2 + math.cos(phi1)*math.cos(phi2)*math.sin(dlambda/2)**2
    return 2 * R * math.atan2(math.sqrt(a), math.sqrt(1-a))

def is_duplicate(lat, lon, threshold=5):
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute("SELECT latitude, longitude FROM potholes")
    rows = c.fetchall()
    conn.close()

    for r in rows:
        if haversine(lat, lon, r[0], r[1]) < threshold:
            return True
    return False

# ✅ Routes
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/dashboard')
def dashboard():
    return render_template('dashboard.html')

# ✅ POST
@app.route('/api/potholes', methods=['POST'])
def add_pothole():
    data = request.json
    lat = data.get('latitude')
    lon = data.get('longitude')
    severity = data.get('severity', 1)

    if lat is None or lon is None:
        return jsonify({'error': 'Invalid data'}), 400

    if not is_duplicate(lat, lon):
        conn = sqlite3.connect(DB_NAME)
        c = conn.cursor()
        c.execute(
            "INSERT INTO potholes (latitude, longitude, severity, timestamp) VALUES (?, ?, ?, ?)",
            (lat, lon, severity, datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        )
        conn.commit()
        conn.close()
        return jsonify({'message': 'Stored'}), 201

    return jsonify({'message': 'Duplicate ignored'}), 200

# ✅ GET
@app.route('/api/potholes', methods=['GET'])
def get_potholes():
    conn = sqlite3.connect(DB_NAME)
    c = conn.cursor()
    c.execute("SELECT latitude, longitude, severity, timestamp FROM potholes")
    rows = c.fetchall()
    conn.close()

    return jsonify([
        {
            'latitude': r[0],
            'longitude': r[1],
            'severity': r[2],
            'timestamp': r[3]
        } for r in rows
    ])

if __name__ == '__main__':
    app.run(host= '0.0.0.0', port=5000)
   
