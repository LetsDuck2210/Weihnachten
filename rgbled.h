#pragma once
#include <Arduino.h>

typedef struct {
  uint8_t pinR, pinG, pinB;
  uint8_t valR, valG, valB;
} RGBLed;

extern void rgbled_tick();
extern void anyPinAnalogWrite(uint8_t pin, int val);
extern void writeRGB(RGBLed led);
extern void setDefaults(RGBLed* leds, int ledcount);
extern void smoothFade(RGBLed* leds, int ledcount, uint8_t rTarget, uint8_t gTarget, uint8_t bTarget);
extern void smoothFade(RGBLed* leds, int ledcount, uint8_t* rTargets, uint8_t* gTargets, uint8_t* bTargets);