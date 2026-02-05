# Progetto acquisizione sensori Arduino + API + DB

Questo repository contiene uno sketch Arduino per inviare i dati di 3 sensori a un'API HTTP e un servizio backend minimale (Flask + SQLite) per salvarli nel database.

## Arduino
Percorso: `arduino/sensor_sender/sensor_sender.ino`

1. Apri lo sketch con Arduino IDE (ESP32).
2. Imposta:
   - `kSsid`
   - `kPassword`
   - `kApiUrl` (es: `http://192.168.1.10:8000/api/v1/measurements`)
3. Collega i sensori ai pin analogici definiti nello sketch.
4. Compila e carica lo sketch.

## Backend API + Database
Percorso: `server/`

1. Crea un virtualenv (opzionale) e installa le dipendenze:
   ```bash
   pip install -r server/requirements.txt
   ```
2. Avvia il server:
   ```bash
   python server/app.py
   ```
3. L'endpoint riceve JSON con questo formato:
   ```json
   {
     "device_id": "arduino-01",
     "timestamp_ms": 123456,
     "temperature_c": 22.5,
     "humidity_percent": 55.2,
     "light_percent": 80.1
   }
   ```

Il database SQLite viene creato automaticamente come `sensor_data.db` nella cartella `server/`.
