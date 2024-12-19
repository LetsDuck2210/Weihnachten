#include "ui.h"

bool buttonDown(uint8_t pin) { return digitalRead(pin); }

Action power_button_down(bool isPoweredOn, uint32_t duration) {
  if (duration >= 2000 || !isPoweredOn)
    return POWER_TOGGLE;
  return NOTHING;
}

ButtonHandler::ButtonHandler(uint8_t button_pin,
                             Action (*button_down_handler)(bool, uint32_t),
                             Action (*button_up_handler)(bool, uint32_t))
    : button_pin(button_pin), button_down_handler(button_down_handler),
      button_up_handler(button_up_handler) {}
Action ButtonHandler::tick(bool isPoweredOn) {
  if (buttonDown(button_pin)) {
    if (button_down_start == 0)
      button_down_start = millis();

    // button_down_handler will only be called until an action is returned,
    // it will then wait until the button is released
    if (last_action != NOTHING)
      return NOTHING;

    if (button_down_handler == nullptr)
      return NOTHING;

    Action action =
        button_down_handler(isPoweredOn, millis() - button_down_start);
    if (action != NOTHING)
      last_action = action;
    return action;
  }

  if (button_down_start == 0)
    return NOTHING;

  // This will only execute when the button was released after being held
  button_down_start = 0;

  Action _last_action = last_action;
  last_action = NOTHING;

  // button_up_handler will only be called if button_down_handler returned no
  // action
  if (_last_action == NOTHING) {
    if (button_up_handler == nullptr)
      return NOTHING;
    return button_up_handler(isPoweredOn, millis() - button_down_start);
  }

  return NOTHING;
}

SingleButtonUI::SingleButtonUI(uint8_t button_pin)
    : handler(button_pin, power_button_down, button_up) {}
Action SingleButtonUI::tick(bool isPoweredOn) { return handler.tick(isPoweredOn); }
Action SingleButtonUI::button_up(bool isPoweredOn, uint32_t duration) {
  return NEXT_PATTERN;
}

DoubleButtonUI::DoubleButtonUI(uint8_t power_button_pin,
                               uint8_t function_button_pin)
    : power_button(power_button_pin, power_button_down, power_button_up),
      function_button(function_button_pin, nullptr, function_button_up) {}
Action DoubleButtonUI::tick(bool isPoweredOn) {
  Action power_action = power_button.tick(isPoweredOn);
  if (power_action != NOTHING)
    return power_action;

  return function_button.tick(isPoweredOn);
}
Action DoubleButtonUI::function_button_up(bool isPoweredOn, uint32_t duration) {
  if (isPoweredOn)
    return NEXT_PATTERN;
  return NOTHING;
}
Action DoubleButtonUI::power_button_up(bool isPoweredOn, uint32_t duration) {
  if(isPoweredOn)
    return SLEEPTIMER_TOGGLE;
  return NOTHING;
}
