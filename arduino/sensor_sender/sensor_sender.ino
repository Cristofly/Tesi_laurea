#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char *kSsid = "YOUR_WIFI_SSID";
const char *kPassword = "YOUR_WIFI_PASSWORD";
const char *kApiUrl = "http://YOUR_SERVER_IP:8000/api/v1/measurements";

const int kDhtPin = 4;
const int kDhtType = DHT22;
const int kLaserPin = 27;
const int kLightPin = 32;

const char *kDeviceId = "arduino-01";

unsigned long kPostIntervalMs = 5000;
unsigned long lastPostMs = 0;

DHT dht(kDhtPin, kDhtType);

float readLightPercent() {
  int raw = analogRead(kLightPin);
  return (raw / 4095.0) * 100.0;
}

int readAccessDetected() {
  return digitalRead(kLaserPin) == HIGH ? 1 : 0;
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(kSsid, kPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(kLaserPin, INPUT_PULLUP);
  dht.begin();
  delay(1000);
  connectWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  unsigned long now = millis();
  if (now - lastPostMs < kPostIntervalMs) {
    delay(100);
    return;
  }
  lastPostMs = now;

  float tempC = dht.readTemperature();
  float humidity = dht.readHumidity();
  int accessDetected = readAccessDetected();
  float light = readLightPercent();

  String payload = String("{") +
                   "\"device_id\":\"" + kDeviceId + "\"," +
                   "\"temperature_c\":" + String(tempC, 2) + "," +
                   "\"humidity_percent\":" + String(humidity, 2) + "," +
                   "\"light_percent\":" + String(light, 2) + "," +
                   "\"access_detected\":" + String(accessDetected) +
                   "}";

  HTTPClient http;
  http.begin(kApiUrl);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload);
  String response = http.getString();

  Serial.print("POST ");
  Serial.print(kApiUrl);
  Serial.print(" -> ");
  Serial.print(httpCode);
  Serial.print(" response: ");
  Serial.println(response);

  http.end();
}
