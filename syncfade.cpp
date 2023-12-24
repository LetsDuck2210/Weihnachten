#pragma once;
#include <Arduino.h>
#include "syncfade.h"
#include "rgbled.h"

#define This SyncFade

This::SyncFade(RGBLed* leds, int ledcount): ColorFade(leds, ledcount) {}

void This::setup() {
  for(int i = 0; i < ledcount; i++) {
    leds[i].valR = 255;
    leds[i].valG = leds[i].valB = 0;
    offsets[i].required = 2;
    writeRGB(leds[i]);
  }
}