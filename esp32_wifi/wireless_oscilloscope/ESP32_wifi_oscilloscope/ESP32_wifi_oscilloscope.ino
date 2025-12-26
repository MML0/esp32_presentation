#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "2.";
const char* password = "12345678an";

IPAddress udpAddress(192,168,43,81);   // PC IP
const int udpPort = 5005;

WiFiUDP udp;
int adcPin = 34;   // Analog pin

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) delay(200);

  udp.begin(udpPort);
}

void loop() {
  int raw = analogRead(adcPin);
  float voltage = raw * (3.3 / 4095.0);   // Convert to voltage

  char msg[32];
  sprintf(msg, "%.3f", voltage);

  udp.beginPacket(udpAddress, udpPort);
  udp.print(msg);
  udp.endPacket();

  delay(20);    // ~50 samples/sec
}
