#include <WiFi.h>

// AP credentials
const char* AP_SSID = "ESP32_AP_Test";
const char* AP_PASSWORD = "12345678";

// AP configuration
IPAddress apIP(192, 168, 10, 1);          // ESP32 IP
IPAddress subnet(255, 255, 255, 0);       // Subnet mask
int channel = 6;                          // Wi-Fi channel (1-13)

// DHCP range (custom)
IPAddress dhcpStart(192, 168, 10, 10);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP32 in AP mode...");

  WiFi.mode(WIFI_AP);

  // Set AP config (IP + subnet)
  if (!WiFi.softAPConfig(apIP, apIP, subnet)) {
    Serial.println("AP IP configuration failed!");
  }

  // Start AP     (ssid, password, channel, hidden, max_connection);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD, channel, false, 4)) {
    Serial.println("Failed to start AP!");
    return;
  }

  Serial.println("AP Started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

}
