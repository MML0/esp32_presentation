#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-AP_led web"; // Soft AP name
const char* password = ""; // Soft AP password

WebServer server(80); // Web server on port 80
const int ledPin = 2; // Built-in LED

// HTML page
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32 SoftAP</title>
  </head>
  <body>
    <h1>Hello World</h1>
    <button onclick="fetch('/on')">Turn ON</button>
    <button onclick="fetch('/off')">Turn OFF</button>
  </body>
</html>
)rawliteral";

// Handlers
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED is ON");
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED is OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Start Soft AP
  WiFi.softAP(ssid, password);
  Serial.println("Soft AP started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Set server routes
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
