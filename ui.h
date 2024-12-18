#include <Arduino.h>

enum Action {
  NOTHING,
  POWER_TOGGLE,
  NEXT_PATTERN,
  SLEEPTIMER_TOGGLE,
};

class UI {
public:
  virtual Action tick(bool isPoweredOn) = 0;
};

class PowerButton {
private:
  uint8_t button_pin;
  uint32_t button_press_start;
  bool button_was_released = true;
public:
  PowerButton(uint8_t pin);
  Action tick(bool isPoweredOn);
};

class SingleButtonUI : public UI {
private:
  uint8_t button_pin;
  bool button_was_pressed = false;
  PowerButton power_button;
public:
  SingleButtonUI(uint8_t button_pin);
  Action tick(bool isPoweredOn);
};

class DoubleButtonUI : public UI {
private:
  uint8_t function_button_pin;
  PowerButton power_button;
public:
  DoubleButtonUI(uint8_t power_button_pin, uint8_t function_button_pin);
  Action tick(bool isPoweredOn);
};
