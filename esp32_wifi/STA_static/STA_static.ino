#include <WiFi.h>
const char* STA_SSID = "YourSSID";
const char* STA_PASSWORD = "YourPassword";
// Static IP configuration
IPAddress local_IP(192, 168, 1, 200);    // ESP32 static IP
IPAddress gateway(192, 168, 1, 1);       // Router IP
IPAddress subnet(255, 255, 255, 0);      // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);        // Primary DNS
IPAddress secondaryDNS(8, 8, 4, 4);      // Secondary DNS (optional)
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to Wi-Fi (Static IP)...");

  WiFi.mode(WIFI_STA);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Static IP Configuration Failed!");
  }

  // Connect to Wi-Fi
  WiFi.begin(STA_SSID, STA_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println("Network Info:");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Primary DNS: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("Secondary DNS: ");
  Serial.println(WiFi.dnsIP(1));
}

void loop() {
  // nothing here
}
