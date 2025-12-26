#include <WiFi.h>

const char* STA_SSID = "2.";
const char* STA_PASSWORD = "12345678an";

// Server to ping / test
const char* host = "mml-dev.ir"; // can be IP too
const uint16_t port = 80;

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

  // --------- Ping Test ---------
  Serial.println("\nPinging server...");
  if (PingTest(host)) {
    Serial.println("Ping successful!");
  } else {
    Serial.println("Ping failed!");
  }

  // --------- Simple TCP GET ---------
  Serial.println("\nPerforming simple HTTP GET...");
  SimpleHTTPGet(host, port, "/?log=esp");
}

void loop() {
  // nothing here
}

// ---------------------- Ping Function ----------------------
bool PingTest(const char* host) {
  // Very simple: try to connect TCP port 80 to check if server reachable
  WiFiClient client;
  if (client.connect(host, 80)) {
    client.stop();
    return true;
  }
  return false;
}

// ---------------------- Simple TCP GET Function ----------------------
void SimpleHTTPGet(const char* host, uint16_t port, const char* path) {
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection failed!");
    return;
  }

  // Send minimal HTTP GET request
  client.print(String("GET ") + path + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read response
  while (client.connected() || client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  client.stop();
  Serial.println("HTTP GET done!");
}
