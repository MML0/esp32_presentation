#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "2.";
const char* password = "12345678an";

const int udpPort = 5005;
WiFiUDP udp;

const int ledPin = 2;          // internal LED (active LOW on most boards)
bool ledState = false;         // software-controlled LED state

const int BUF_SIZE = 500;
uint8_t buf[BUF_SIZE];

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF (active LOW)

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected.");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  udp.begin(udpPort);
  Serial.println("UDP listening...");
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
    int len = udp.read(buf, BUF_SIZE);
    if (len <= 0) return;

    // -------- Update LED STATE ONLY --------
    ledState = (buf[0] % 2 == 0);   // true = ON, false = OFF
    digitalWrite(ledPin, ledState ? LOW : HIGH); // LOW = ON (active LOW)
    delay(5000);
  }

  // -------- Apply LED STATE here --------
  digitalWrite(ledPin, ledState ? LOW : HIGH); // LOW = ON (active LOW)
}
