#pragma once
#include "pattern.h"

typedef struct {
  uint8_t required, current;
} Offset;

class ColorFade: public Pattern {
protected:
  RGBLed* leds;
  Offset* offsets;
  int ledcount;

  ColorFade();
  void shift(RGBLed* led, Offset* offset);
public:
  ColorFade(RGBLed* leds, int ledcount);
  void setup();
  void tick();
};