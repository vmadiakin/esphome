#pragma once
#include "esphome.h"

// Класс кастомного компонента для 3x4 панели
class KeypadComponent : public Component {
 public:
  // Пины строк (Rows) и столбцов (Cols)
  std::vector<int> row_pins;
  std::vector<int> col_pins;

  // Реле управления замком (подключаем через switch ESPHome)
  switch_::Switch *lock;

  // Локальные коды, которые разрешают открытие замка
  std::vector<String> codes = {"1234","5678"};

  // Текущий введенный код
  String current_code = "";

  void setup() override {
    // Настраиваем строки как входы с подтяжкой
    for(auto &r: row_pins){
      pinMode(r, INPUT_PULLUP);
    }
    // Столбцы как выходы и поднимаем HIGH
    for(auto &c: col_pins){
      pinMode(c, OUTPUT);
      digitalWrite(c, HIGH);
    }
  }

  void loop() override {
    // Сканируем панель
    for(size_t c=0; c<col_pins.size(); c++){
      digitalWrite(col_pins[c], LOW); // активируем столбец
      for(size_t r=0; r<row_pins.size(); r++){
        if(digitalRead(row_pins[r]) == LOW){ // кнопка нажата
          char key = get_key(r,c);
          handle_key(key);
          delay(200); // антидребезг
        }
      }
      digitalWrite(col_pins[c], HIGH); // отключаем столбец
    }
  }

  // Получаем символ нажатой кнопки
  char get_key(int row, int col){
    // 3x4 панель как на телефоне
    const char keys[4][3] = {
      {'1','2','3'},  // Row1
      {'4','5','6'},  // Row2
      {'7','8','9'},  // Row3
      {'*','0','#'}   // Row4
    };
    return keys[row][col];
  }

  // Обработка нажатой кнопки
  void handle_key(char key){
    if(key == '*'){ 
      current_code = ""; // очистка кода
      return;
    }

    if(key != '#'){
      current_code += key;
    }

    // Когда введено 4 цифры — проверяем
    if(current_code.length() == 4){ 
      bool ok = false;
      for(auto &c: codes){
        if(c == current_code) ok = true;
      }

      if(ok){
        ESP_LOGD("keypad","Code correct: %s", current_code.c_str());
        if(lock) lock->turn_on();       // включаем реле
        delay(3000);                     // держим 3 секунды
        if(lock) lock->turn_off();      // отключаем реле
      } else {
        ESP_LOGD("keypad","Wrong code: %s", current_code.c_str());
      }
      current_code = "";
    }
  }
};
