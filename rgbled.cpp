#pragma once
#include "rgbled.h"

void writeRGB(RGBLed led) {
  analogWrite(led.pinR, led.valR);
  analogWrite(led.pinG, led.valG);
  analogWrite(led.pinB, led.valB);
}

void setDefaults(RGBLed *leds, int ledcount) {
  for(int i = 0; i < ledcount; i++) {
    uint8_t r[] = {255, 0, 0},  //  v
            g[] = {0, 255, 0},  //  
            b[] = {0, 0, 255};  //  prevent exact synchronization
    leds[i].valR = r[i];        //  
    leds[i].valG = g[i];        //  
    leds[i].valB = b[i];        //  ^
    writeRGB(leds[i]);
  }
}