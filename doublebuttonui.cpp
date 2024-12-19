#include "ui.h"

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
