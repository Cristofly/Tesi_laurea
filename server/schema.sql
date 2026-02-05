CREATE TABLE IF NOT EXISTS measurements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id TEXT NOT NULL,
    temperature_c REAL,
    humidity_percent REAL,
    light_percent REAL,
    access_detected INTEGER NOT NULL DEFAULT 0,
    captured_at TEXT NOT NULL
);
