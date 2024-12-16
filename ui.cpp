#include "ui.h"

bool buttonDown(uint8_t pin) {
  return digitalRead(pin);
}

SingleButtonUI::SingleButtonUI(uint8_t button_pin) : button_pin(button_pin) { }
Action SingleButtonUI::tick(bool isPoweredOn) {
  if(buttonDown(button_pin)) {
    if(isPoweredOn) {
      uint32_t start = millis();
      while(buttonDown(button_pin)) {
        uint32_t duration = millis() - start;
        if(duration > 2000)
          return POWER_TOGGLE;
      }
      return NOTHING;
    }

    return POWER_TOGGLE;
  }
  
  return NOTHING;
}



DoubleButtonUI::DoubleButtonUI(uint8_t power_button_pin, uint8_t function_button_pin) 
  : SingleButtonUI(power_button_pin), function_button_pin(function_button_pin) { }

Action DoubleButtonUI::tick(bool isPoweredOn) {
  Action powerButtonAction = SingleButtonUI::tick(isPoweredOn);
  if(powerButtonAction != NOTHING)
    return powerButtonAction;

  if(buttonDown(function_button_pin)) {
    while(buttonDown(function_button_pin)) delay(10);
    
    return NEXT_PATTERN;
  }

  return NOTHING;
}
