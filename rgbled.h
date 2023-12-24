#pragma once
#include <Arduino.h>

typedef struct {
  uint8_t pinR, pinG, pinB;
  uint8_t valR, valG, valB;
} RGBLed;

extern void writeRGB(RGBLed led);
extern void setDefaults(RGBLed* leds, int ledcount);