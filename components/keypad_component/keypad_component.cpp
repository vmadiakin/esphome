#include "keypad_component.h"
#include "esphome/core/log.h"

static const char *TAG = "keypad_component";

// Классическая раскладка 3x4
static const char KEYS[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void KeypadComponent::setup() {
  ESP_LOGI(TAG, "Initializing keypad");

  // строки — выходы
  for (auto pin : row_pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // столбцы — входы с подтяжкой
  for (auto pin : col_pins) {
    pinMode(pin, INPUT_PULLUP);
  }

  ESP_LOGI(TAG, "Keypad ready: %d rows, %d cols",
           row_pins.size(), col_pins.size());
}

void KeypadComponent::loop() {
  for (size_t r = 0; r < row_pins.size(); r++) {
    digitalWrite(row_pins[r], LOW);

    for (size_t c = 0; c < col_pins.size(); c++) {
      if (digitalRead(col_pins[c]) == LOW) {
        char key = KEYS[r][c];
        ESP_LOGI(TAG, "Key pressed: %c", key);

        // Пример: # — открыть замок
        if (key == '#' && lock != nullptr) {
          ESP_LOGI(TAG, "Unlock trigger");
          lock->digital_write(true);
          delay(1000);
          lock->digital_write(false);
        }

        // антидребезг
        delay(300);
      }
    }

    digitalWrite(row_pins[r], HIGH);
  }
}
