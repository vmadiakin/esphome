#pragma once
#include "esphome/core/component.h"

using namespace esphome;

class KeypadComponent : public Component {
 public:
  std::vector<int> row_pins;
  std::vector<int> col_pins;
  GPIOPin *lock;

  void setup() override;
  void loop() override;
};
