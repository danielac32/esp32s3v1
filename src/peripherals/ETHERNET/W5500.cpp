#include "W5500.h"


#define AP_SSID "ESP32S3"
#define AP_PASS "password"

IPAddress ap_ip(192, 168, 4, 1);
IPAddress ap_mask(255, 255, 255, 0);
IPAddress ap_leaseStart(192, 168, 4, 2);
IPAddress ap_dns(8, 8, 4, 4);


W55::W55(uint8_t cs,uint8_t irq,uint8_t rst,uint8_t sck,uint8_t miso,uint8_t mosi):
_cs(cs),_irq(irq),_rst(rst),_sck(sck),_miso(miso),_mosi(mosi){
}

void W55::begin(){
  Serial.setDebugOutput(true);
  Network.onEvent(W55::onEvent);  // Usa W55::onEvent
  WiFi.AP.begin();
  WiFi.AP.config(ap_ip, ap_ip, ap_mask, ap_leaseStart, ap_dns);
  WiFi.AP.create(AP_SSID, AP_PASS);
  if (!WiFi.AP.waitStatusBits(ESP_NETIF_STARTED_BIT, 1000)) {
    Serial.println("Failed to start AP!");
    return;
  }
  delay(100);

  SPI.begin(W55::_sck, W55::_miso, W55::_mosi);
  ETH.begin(ETH_PHY_W5500, 1, W55::_cs, W55::_irq, W55::_rst, SPI);
}


void W55::onEvent(arduino_event_id_t event, arduino_event_info_t info) {
  switch (event) {
    case ARDUINO_EVENT_ETH_START:     Serial.println("ETH Started"); break;
    case ARDUINO_EVENT_ETH_CONNECTED: Serial.println("ETH Connected"); break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.println("ETH Got IP");
      Serial.println(ETH);
      WiFi.AP.enableNAPT(true);
      break;
    case ARDUINO_EVENT_ETH_LOST_IP:
      Serial.println("ETH Lost IP");
      WiFi.AP.enableNAPT(false);
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      WiFi.AP.enableNAPT(false);
      break;
    case ARDUINO_EVENT_ETH_STOP: Serial.println("ETH Stopped"); break;

    case ARDUINO_EVENT_WIFI_AP_START:
      Serial.println("AP Started");
      Serial.println(WiFi.AP);
      break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    Serial.println("AP STA Connected"); break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("AP STA Disconnected"); break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
      Serial.print("AP STA IP Assigned: ");
      Serial.println(IPAddress(info.wifi_ap_staipassigned.ip.addr));
      break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED: Serial.println("AP Probe Request Received"); break;
    case ARDUINO_EVENT_WIFI_AP_STOP:           Serial.println("AP Stopped"); break;

    default: break;
  }
}