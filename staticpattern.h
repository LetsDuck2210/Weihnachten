#pragma once
#include "pattern.h"

class StaticPattern: public Pattern {
protected:
  RGBLed* leds;
  int ledcount;

  StaticPattern();
public:
  StaticPattern(RGBLed* leds, int ledcount);
  void setup();
};