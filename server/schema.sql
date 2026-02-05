CREATE TABLE IF NOT EXISTS measurements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id TEXT NOT NULL,
    temperature_c REAL,
    humidity_percent REAL,
    light_percent REAL,
    timestamp_ms INTEGER,
    captured_at TEXT NOT NULL
);
