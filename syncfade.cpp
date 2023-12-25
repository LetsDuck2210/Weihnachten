#pragma once;
#include <Arduino.h>
#include "syncfade.h"
#include "rgbled.h"

#define This SyncFade

This::SyncFade(RGBLed* leds, int ledcount): ColorFade(leds, ledcount) {}

void This::setup() {
  smoothFade(leds, ledcount, 255, 0, 0);
  for(int i = 0; i < ledcount; i++) {
    offsets[i].required = 0;
  }
}