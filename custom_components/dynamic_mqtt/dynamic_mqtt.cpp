#include "dynamic_mqtt.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace dynamic_mqtt {

static const char *const TAG = "dynamic_mqtt";

void DynamicMQTTComponent::setup() {
  // Získání MAC adresy pomocí ESPHome API
  uint8_t mac[6];
  get_mac_address_raw(mac);
  
  // Konverze na string bez dvojteček
  char mac_str[13];
  sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", 
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  // Nastavení topic prefixu s MAC adresou
  mac_topic_prefix_ = "esp32device/" + std::string(mac_str);
  
  ESP_LOGI(TAG, "Setting MQTT topic prefix to: %s", mac_topic_prefix_.c_str());
  
  // Nastavení topic prefixu pro MQTT klienta
  if (mqtt_client_ != nullptr) {
    // Nastavení topic prefixu - vyžaduje 2 parametry
    mqtt_client_->set_topic_prefix(mac_topic_prefix_, mac_topic_prefix_);
    
    // Aktualizace birth zprávy - používáme std::move pro rvalue reference
    mqtt_client_->set_birth_message({
        .topic = mac_topic_prefix_ + "/status",
        .payload = "online",
        .qos = 0,
        .retain = true,
    });
    
    // Aktualizace will zprávy - používáme std::move pro rvalue reference
    mqtt_client_->set_last_will({
        .topic = mac_topic_prefix_ + "/status",
        .payload = "offline",
        .qos = 0,
        .retain = true,
    });
  }
}

}  // namespace dynamic_mqtt
}  // namespace esphome 