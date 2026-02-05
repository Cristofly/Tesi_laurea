from datetime import datetime
import os
import sqlite3

from flask import Flask, jsonify, request

DB_PATH = os.environ.get("SENSOR_DB_PATH", "sensor_data.db")

app = Flask(__name__)


def get_connection():
    connection = sqlite3.connect(DB_PATH)
    connection.row_factory = sqlite3.Row
    return connection


def init_db():
    schema_path = os.path.join(os.path.dirname(__file__), "schema.sql")
    with open(schema_path, "r", encoding="utf-8") as schema_file:
        schema = schema_file.read()
    with get_connection() as connection:
        connection.executescript(schema)


@app.route("/api/v1/measurements", methods=["POST"])
def create_measurement():
    payload = request.get_json(silent=True) or {}

    device_id = payload.get("device_id")
    temperature_c = payload.get("temperature_c")
    humidity_percent = payload.get("humidity_percent")
    light_percent = payload.get("light_percent")
    access_detected = payload.get("access_detected", 0)

    if not device_id:
        return jsonify({"error": "device_id is required"}), 400

    captured_at = datetime.utcnow().isoformat()

    with get_connection() as connection:
        connection.execute(
            """
            INSERT INTO measurements (
                device_id,
                temperature_c,
                humidity_percent,
                light_percent,
                access_detected,
                captured_at
            ) VALUES (?, ?, ?, ?, ?, ?)
            """,
            (
                device_id,
                temperature_c,
                humidity_percent,
                light_percent,
                access_detected,
                captured_at,
            ),
        )
        connection.commit()

    return jsonify({"status": "ok", "captured_at": captured_at}), 201


@app.route("/health", methods=["GET"])
def health_check():
    return jsonify({"status": "ok"})


if __name__ == "__main__":
    init_db()
    app.run(host="0.0.0.0", port=8000)
