#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const char* STA_SSID = "2.";
const char* STA_PASSWORD = "12345678an";

WiFiUDP udp;
const int localPort = 12345;
const int ledPin = 2;  // built-in LED on most ESP32 boards

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED off initially

  // WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASSWORD);

  udp.begin(localPort);
  Serial.print("UDP server started at ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  char packetBuffer[255];
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.print("Received: ");
    Serial.println(packetBuffer);

    // Check first byte for 0xFF
    if ((uint8_t)packetBuffer[0] == 0xFF) {
      Serial.println("Turning LED ON");
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }

    // Optional: send reply
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    // udp.write("Hello from ESP32!");
    udp.write((const uint8_t*)"Hello from ESP32!", strlen("Hello from ESP32!"));
    udp.endPacket();
  }
}
