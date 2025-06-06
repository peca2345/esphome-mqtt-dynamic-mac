// custom_components/dynamic_mqtt/dynamic_mqtt.h
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace dynamic_mqtt {

class DynamicMQTTComponent : public Component {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::AFTER_CONNECTION; }
  void set_mqtt_client(mqtt::MQTTClientComponent *mqtt_client) { mqtt_client_ = mqtt_client; }
  std::string get_mac_topic_prefix() { return mac_topic_prefix_; }
  
 protected:
  mqtt::MQTTClientComponent *mqtt_client_{nullptr};
  std::string mac_topic_prefix_;
};

}  // namespace dynamic_mqtt
}  // namespace esphome