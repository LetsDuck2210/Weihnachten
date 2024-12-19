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

Action power_button_down(bool isPoweredOn, uint32_t duration);

class ButtonHandler {
private:
  uint8_t button_pin;
  uint32_t button_down_start = 0;
  Action last_action = NOTHING;
  Action (*button_down_handler)(bool, uint32_t),
      (*button_up_handler)(bool, uint32_t);

public:
  ButtonHandler(uint8_t button_pin,
                Action (*button_down_handler)(bool isOn, uint32_t duration),
                Action (*button_up_handler)(bool isOn, uint32_t duration));
  Action tick(bool isPoweredOn);
};

class SingleButtonUI : public UI {
private:
  ButtonHandler handler;

public:
  SingleButtonUI(uint8_t button_pin);
  Action tick(bool isPoweredOn);

private:
  static Action button_up(bool isPoweredOn, uint32_t duration);
};

class DoubleButtonUI : public UI {
private:
  ButtonHandler power_button;
  ButtonHandler function_button;

public:
  DoubleButtonUI(uint8_t power_button_pin, uint8_t function_button_pin);
  Action tick(bool isPoweredOn);

private:
  static Action function_button_up(bool isPoweredOn, uint32_t duration);
};
