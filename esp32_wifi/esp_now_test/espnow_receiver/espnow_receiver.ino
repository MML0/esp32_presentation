#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  int value;
} struct_message;

struct_message incomingData;

void onDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  Serial.println(incomingData.value);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Nothing here, data handled by callback
}
