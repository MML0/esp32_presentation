#include <WiFi.h>
#include <HTTPClient.h>

// ====== Wi-Fi Credentials ======
const char* STA_SSID     = "2.";
const char* STA_PASSWORD = "12345678an";

// ====== Server Log URL (HTTP ONLY) ======
const char* logURLBase = "http://mml-dev.ir/?log=";

// ====== LM35 Settings ======
const int lm35Pin = 34;        // ADC pin
const float adcRef = 3.3;      // ESP32 ADC reference
const int adcMax = 4095;       // 12-bit ADC

// ====== Timing ======
unsigned long previousMillis = 0;
const unsigned long interval = 10000; // 10 seconds

// ====== WiFi Client (IMPORTANT) ======
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(500);

  // ===== ADC setup =====
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db); // ~0–3.3V

  // ===== Wi-Fi =====
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(STA_SSID, STA_PASSWORD);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startAttemptTime > 15000) {
      Serial.println("\nWiFi FAILED");
      return;
    }
  }

  Serial.println("\nWi-Fi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    float tempC = readLM35();

    Serial.print("Temp (°C): ");
    Serial.println(tempC, 2);

    sendTemperatureToServer(tempC);
  }
}

// ====== READ LM35 ======
float readLM35() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(lm35Pin);
    delay(5);
  }

  float raw = sum / 10.0;
  float voltage = raw * (adcRef / adcMax);
  return voltage * 100.0;
}

// ====== HTTP GET LOGGER ======
void sendTemperatureToServer(float value) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi not connected");
    return;
  }

  HTTPClient http;

  String url = String(logURLBase) + String(value, 2);

  Serial.print("Sending to URL: ");
  Serial.println(url);

  // ⭐ THIS IS THE IMPORTANT PART ⭐
  if (!http.begin(client, url)) {
    Serial.println("HTTP begin failed");
    return;
  }

  http.setTimeout(5000);
  http.setUserAgent("ESP32");
  http.setReuse(false); // avoid socket issues

  int httpCode = http.GET();

  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.print("Server response: ");
    Serial.println(payload);
  } else {
    Serial.print("HTTP error: ");
    Serial.println(http.errorToString(httpCode));
  }

  http.end();
}
