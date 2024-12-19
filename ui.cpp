#include "ui.h"
#include "Arduino.h"

bool buttonDown(uint8_t pin) {
#if USE_PULLUP == 1
  return !digitalRead(pin);
#else
  return digitalRead(pin);
#endif
}

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
