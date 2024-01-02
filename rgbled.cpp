#pragma once
#include "rgbled.h"
#include "Arduino.h"

#define ANALOG_COUNT 8
int values[ANALOG_COUNT];
unsigned long times[ANALOG_COUNT];
void anyPinAnalogWrite(uint8_t pin, int val) {
  if(pin >= A0 && pin <= A7) {
    values[pin - A0] = map(val, 0, 255, 1000, 0);
    times[pin - A0] = micros();
  } else {
    analogWrite(pin, val);
  }
}
void rgbled_tick() {
  for(uint8_t i = 0; i < ANALOG_COUNT; i++) {
    uint8_t pin = i + A0;
    if(values[i] >= 850) {
      digitalWrite(pin, LOW);
      continue;
    }
    if(values[i] == 0) {
      digitalWrite(pin, HIGH);
      continue;
    }
    unsigned long current = micros() - times[i];
    if(current >= values[i])
      digitalWrite(pin, HIGH);
    if(current > 1000) {
      digitalWrite(pin, LOW);
      times[i] = micros();
    }
  }
}

void writeRGB(RGBLed led) {
  anyPinAnalogWrite(led.pinR, led.valR);
  anyPinAnalogWrite(led.pinG, led.valG);
  anyPinAnalogWrite(led.pinB, led.valB);
}

void setDefaults(RGBLed* leds, int ledcount) {
  uint8_t r[ledcount], g[ledcount], b[ledcount];

  // prevent exact synchronization
  for(int i = 0; i < ledcount; i++) {
    float s = (1.0 * i) / ledcount; // 1.0 * i  ->  i must be float
    Serial.println("s: " + String(s));
    if(s >= 0.5) {
      r[i] = 0;
      uint8_t g0 = (s - 0.5) * 255;
      uint8_t b0 = s * 255;

      g[i] = round(g0 * (255.0 / max(g0, b0))); // v
      b[i] = round(b0 * (255.0 / max(g0, b0))); // v
    } else {
      uint8_t r0 = (0.5 - s) * 255;
      uint8_t g0 = s * 255;
      b[i] = 0;

      r[i] = round(r0 * (255.0 / max(r0, g0))); // v
      g[i] = round(g0 * (255.0 / max(r0, g0))); // one must be 255
    }
  }
  
  smoothFade(leds, ledcount, r, g, b);
}

uint8_t lerp(uint8_t start, uint8_t end, float t) {
  if(start == end) return start;
  return (1 - t) * start + t * end;
}

void smoothFade(RGBLed* leds, int ledcount, uint8_t rTarget, uint8_t gTarget, uint8_t bTarget) {
  uint8_t r[ledcount], g[ledcount], b[ledcount];
  for(int i = 0; i < ledcount; i++) {
    r[i] = rTarget;
    g[i] = gTarget;
    b[i] = bTarget;
  }
  smoothFade(leds, ledcount, r, g, b);
}
void smoothFade(RGBLed* leds, int ledcount, uint8_t* rTargets, uint8_t* gTargets, uint8_t* bTargets) {
  uint8_t rStarts[ledcount],
          gStarts[ledcount],
          bStarts[ledcount];
  for(int i = 0; i < ledcount; i++) {
    rStarts[i] = leds[i].valR;
    gStarts[i] = leds[i].valG;
    bStarts[i] = leds[i].valB;
  }
  for(int i = 0; i <= 100; i++) {
    float t = i / 100.0;
    for(int j = 0; j < ledcount; j++) {
      leds[j].valR = lerp(rStarts[j], rTargets[j], t);
      leds[j].valG = lerp(gStarts[j], gTargets[j], t);
      leds[j].valB = lerp(bStarts[j], bTargets[j], t);
      writeRGB(leds[j]);
    }
    delay(2);
  }
}