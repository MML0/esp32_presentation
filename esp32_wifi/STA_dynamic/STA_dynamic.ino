#include <WiFi.h>
const char* STA_SSID = "YourSSID";
const char* STA_PASSWORD = "YourPassword";
void setup() {
  Serial.begin(115200);
  delay(1000);
  // --------- Connect to Wi-Fi ---------
  Serial.print("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Network Info:");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Primary DNS: ");
  Serial.println(WiFi.dnsIP());
}
void loop() {
  // nothing here
}
