#include "ui.h"

SingleButtonUI::SingleButtonUI(uint8_t button_pin)
    : handler(button_pin, power_button_down, button_up) {}
Action SingleButtonUI::tick(bool isPoweredOn) { return handler.tick(isPoweredOn); }
Action SingleButtonUI::button_up(bool isPoweredOn, uint32_t duration) {
  return NEXT_PATTERN;
}
