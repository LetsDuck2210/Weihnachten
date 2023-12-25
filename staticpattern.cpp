#pragma once
#include "staticpattern.h"
#include "rgbled.h"

#define This StaticPattern

This::StaticPattern(RGBLed* leds, int ledcount) : leds(leds), ledcount(ledcount) {}




void This::setup() {
  smoothFade(leds, ledcount, 40, 255, 255);
}
