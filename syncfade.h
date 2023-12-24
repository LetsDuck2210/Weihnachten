#pragma once
#include "pattern.h"
#include "colorfade.h"

class SyncFade: public ColorFade {
public:
  SyncFade(RGBLed* leds, int ledcount);
  void setup();
};