#include "ui.h"

bool buttonDown(uint8_t pin) {
  return digitalRead(pin);
}
void waitForRelease(uint8_t pin) {
  while(buttonDown(pin)) delay(10);
}

PowerButton::PowerButton(uint8_t pin): button_pin(pin) {}
Action PowerButton::tick(bool isPoweredOn) {
  if(buttonDown(button_pin)) {
    if(isPoweredOn) {
      if(button_press_start == 0)
        button_press_start = millis();

      if(button_was_released && millis() - button_press_start >= 2000) {
        button_was_released = false;
        return POWER_TOGGLE;
      }
      return NOTHING;
    }

    if(button_was_released)
      return POWER_TOGGLE;
    return NOTHING;
  }

  button_press_start = 0;
  button_was_released = true;
  
  return NOTHING;
}

SingleButtonUI::SingleButtonUI(uint8_t button_pin) : button_pin(button_pin), power_button(button_pin) { }
Action SingleButtonUI::tick(bool isPoweredOn) {
  Action powerButtonAction = power_button.tick(isPoweredOn);
  if(powerButtonAction != NOTHING) {
    button_was_pressed = false;
    return powerButtonAction;
  }

  if(buttonDown(button_pin)) {
    button_was_pressed = true;
    return NOTHING;
  }
  if(button_was_pressed) {
    button_was_pressed = false;
    return NEXT_PATTERN;
  }

  return NOTHING;
}



DoubleButtonUI::DoubleButtonUI(uint8_t power_button_pin, uint8_t function_button_pin) 
  : function_button_pin(function_button_pin), power_button(power_button_pin) { }

Action DoubleButtonUI::tick(bool isPoweredOn) {
  Action powerButtonAction = power_button.tick(isPoweredOn);
  if(powerButtonAction != NOTHING)
    return powerButtonAction;

  if(isPoweredOn && buttonDown(function_button_pin)) {
    waitForRelease(function_button_pin);
    
    return NEXT_PATTERN;
  }

  return NOTHING;
}
