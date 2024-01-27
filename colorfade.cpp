#pragma once
#include <Arduino.h>
#include "colorfade.h"
#include "rgbled.h"

#define This ColorFade

This::ColorFade(RGBLed* leds, int ledcount) : leds(leds), ledcount(ledcount) {
  offsets = new Offset[ledcount];
}
This::ColorFade() {}

void This::setup() {
  setDefaults(leds, ledcount);
  for(int i = 0; i < ledcount; i++) {
    offsets[i].required = random(4) + 1;
  }
}

void This::shift(RGBLed* led, Offset* offset) {
  if(offset->current < offset->required) { // only process every requiredOffset-th shift
    offset->current++;
    return;
  }
  offset->current = 0;

  if(led->valR < 255 && led->valG == 0 && led->valB == 0)   // (all zero -> fade in red)
    led->valR = min(led->valR + 4, 255);
  if(led->valR == 255 && led->valG < 255 && led->valB == 0) // red on -> fade in green
    led->valG = min(led->valG + 4, 255);
  if(led->valR > 0 && led->valG == 255 && led->valB == 0)   // green on -> fade out red
    led->valR = max(led->valR - 4, 0);
  if(led->valR == 0 && led->valG == 255 && led->valB < 255) // green on -> fade in blue
    led->valB = min(led->valB + 4, 255);
  if(led->valR == 0 && led->valG > 0 && led->valB == 255)   // blue on -> fade out green
    led->valG = max(led->valG - 4, 0);
  if(led->valR < 255 && led->valG == 0 && led->valB == 255) // blue on -> fade in red
    led->valR = min(led->valR + 4, 255);
  if(led->valR == 255 && led->valG == 0 && led->valB > 0)   // red on -> fade out blue
    led->valB = max(led->valB - 4, 0);

  writeRGB(*led);
}
void This::tick() {
  for(int i = 0; i < ledcount; i++) {
    shift(&leds[i], &offsets[i]);
  }
}