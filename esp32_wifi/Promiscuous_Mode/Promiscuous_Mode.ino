#include <WiFi.h>
#include "esp_wifi.h"

typedef struct {
  uint8_t frame_ctrl;
  uint8_t duration_id;
  uint8_t addr1[6];
  uint8_t addr2[6];
  uint8_t addr3[6];
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[];
} wifi_ieee80211_packet_t;

// Convert MAC to text
String macToStr(const uint8_t *mac)
{
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

// Sniffer callback
void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
  if (type == WIFI_PKT_MISC) return;

  wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buff;
  wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)pkt->payload;
  wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  Serial.println("\n--- PACKET ---");
  Serial.print("Type: "); Serial.println(type);

  Serial.print("RSSI: "); Serial.println(pkt->rx_ctrl.rssi);

  Serial.print("Src MAC: ");
  Serial.println(macToStr(hdr->addr2));

  Serial.print("Dst MAC: ");
  Serial.println(macToStr(hdr->addr1));

  Serial.print("Length: ");
  Serial.println(pkt->rx_ctrl.sig_len);

  Serial.print("Channel: ");
  Serial.println(pkt->rx_ctrl.channel);
  Serial.print("Payload: ");
  for (int i = 0; i < pkt->rx_ctrl.sig_len; i++) {
      Serial.printf("%02X ", ipkt->payload[i]);
  }
  Serial.println();

}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStarting ESP32 WiFi Sniffer...");

  WiFi.mode(WIFI_MODE_NULL);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_filter(NULL);

  // Register callback
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);

  // Start Wi-Fi
  esp_wifi_start();

  // Hop channels (optional)
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.println("Promiscuous mode ON");
}

void loop()
{
  static uint8_t ch = 1;
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  Serial.printf("Scanning channel %d\n", ch);

  ch++;
  if (ch > 13) ch = 1;

  delay(1000);
}
