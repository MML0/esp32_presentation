#include "WiFi.h"
#include "esp_wifi.h"

int channelToSniff = 11;

void snifferPacketHandler(void* buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t*)buf;
    uint8_t *data = pkt->payload;
    uint16_t len = pkt->rx_ctrl.sig_len;

    Serial.write(0xFE);
    Serial.write(len & 0xFF);
    Serial.write(len >> 8);
    Serial.write(data, len);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    // Correct WiFi mode
    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(snifferPacketHandler);

    wifi_promiscuous_filter_t filt = {};
    filt.filter_mask = WIFI_PROMIS_FILTER_MASK_ALL;
    esp_wifi_set_promiscuous_filter(&filt);

    esp_wifi_set_channel(channelToSniff, WIFI_SECOND_CHAN_NONE);
    Serial.println("Sniffer started.");
}

void loop() {
    // nothing – handled by callback
}
