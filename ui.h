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

class SingleButtonUI : public UI {
private:
  uint8_t button_pin;
public:
  SingleButtonUI(uint8_t button_pin);
  Action tick(bool isPoweredOn);
};

class DoubleButtonUI : public SingleButtonUI {
private:
  uint8_t function_button_pin;
public:
  DoubleButtonUI(uint8_t power_button_pin, uint8_t function_button_pin);
  Action tick(bool isPoweredOn);
};
