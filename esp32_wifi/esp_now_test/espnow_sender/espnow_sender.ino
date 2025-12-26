#include <WiFi.h>
#include <esp_now.h>

int adcPin = 34; // Analog pin

typedef struct struct_message {
  int value;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Required for ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // Broadcast to all devices
  memcpy(peerInfo.peer_addr, "\xFF\xFF\xFF\xFF\xFF\xFF", 6); 
  esp_now_add_peer(&peerInfo);
}

void loop() {
  myData.value = analogRead(adcPin); // read ADC
  esp_err_t result = esp_now_send(NULL, (uint8_t *)&myData, sizeof(myData));
  if (result == ESP_OK) Serial.println("Sent");
  else Serial.println("Send failed");
  delay(50); // ~20 Hz
}
