#pragma once
#include "rgbled.h"

void writeRGB(RGBLed led) {
  analogWrite(led.pinR, led.valR);
  analogWrite(led.pinG, led.valG);
  analogWrite(led.pinB, led.valB);
}