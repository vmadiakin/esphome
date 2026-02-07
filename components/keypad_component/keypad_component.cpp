#include "keypad_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace keypad_component {

static const char *TAG = "keypad";

const char KEYMAP[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void KeypadComponent::setup() {
  ESP_LOGI(TAG, "Setting up keypad");

  for (auto pin : row_pins_) {
    pin->setup();
    pin->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  }

  for (auto pin : col_pins_) {
    pin->setup();
    pin->digital_write(true);
    pin->pin_mode(gpio::FLAG_OUTPUT);
  }
}

void KeypadComponent::loop() {
  for (size_t c = 0; c < col_pins_.size(); c++) {
    col_pins_[c]->digital_write(false);

    for (size_t r = 0; r < row_pins_.size(); r++) {
      if (!row_pins_[r]->digital_read()) {
        char key = KEYMAP[r][c];
        handle_key_(key);
        delay(250);  // антидребезг
      }
    }

    col_pins_[c]->digital_write(true);
  }
}

void KeypadComponent::handle_key_(char key) {
  ESP_LOGD(TAG, "Key pressed: %c", key);

  if (key == '#') {
    if (entered_code_ == stored_code_) {
      ESP_LOGI(TAG, "Correct code → opening lock");
      lock_->turn_on();
      delay(3000);
      lock_->turn_off();
    } else {
      ESP_LOGW(TAG, "Wrong code");
    }
    entered_code_.clear();
    return;
  }

  if (key == '*') {
    entered_code_.clear();
    return;
  }

  entered_code_ += key;
}

}  // namespace keypad_component
}  // namespace esphome
