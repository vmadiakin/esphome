#pragma once
#include "esphome/core/component.h"
#include "esphome/components/gpio/output.h"
#include <string>
#include <vector>

namespace esphome {
namespace keypad_component {

class KeypadComponent : public Component {
 public:
  std::vector<esphome::GPIOPin*> row_pins_;
  std::vector<esphome::GPIOPin*> col_pins_;
  esphome::gpio::GPIOOutputPin *lock_{nullptr};

  std::string entered_code_;  // буфер ввода кода

  // Управление кодами через внешний источник (HA/MQTT)
  std::string current_code_;

  void setup() override;
  void loop() override;

 protected:
  void handle_key_(char key);
};

}  // namespace keypad_component
}  // namespace esphome
