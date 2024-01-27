#include "rgbled.h"
#include "pattern.h"
#include "colorfade.h"
#include "syncfade.h"
#include "staticpattern.h"
#include <Timer.h>

#define u8 (uint8_t)
#define SLEEPTIMER (3 * 60 * 60)  // 3 hours
#define PWR_BUTTON_PIN 2          // button to control power (on/off/sleeptimer)
#define FUNCTION_BUTTON_PIN 4     // button to control the running program
#define RGBLED_COUNT 3
RGBLed RGBLEDs[RGBLED_COUNT] = {
  { A2, 5, 6,    255, 0, 0 },
  { 11, 10, 9,  0, 255, 0 },
  { A0, 3, A1, 0, 0, 255 }
};
bool isOn = true;

#define PATTERN_COUNT 3
Pattern* patterns[PATTERN_COUNT] = {
  new ColorFade(RGBLEDs, RGBLED_COUNT),
  new SyncFade(RGBLEDs, RGBLED_COUNT),
  new StaticPattern(RGBLEDs, RGBLED_COUNT)
};
int currentPattern = 0;
Timer loopTimer;
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  pinMode(PWR_BUTTON_PIN, INPUT);
  pinMode(FUNCTION_BUTTON_PIN, INPUT);
  for (int i = 0; i < RGBLED_COUNT; i++) {
    pinMode(RGBLEDs[i].pinR, OUTPUT);
    pinMode(RGBLEDs[i].pinG, OUTPUT);
    pinMode(RGBLEDs[i].pinB, OUTPUT);
  }

  patterns[currentPattern]->setup();

  loopTimer.start();
}

Timer sleeptimer;
void togglePwr() {
  isOn = !isOn;
  if(!isOn) {
    smoothFade(RGBLEDs, RGBLED_COUNT, u8 0, u8 0, u8 0);
  } else {
    setDefaults(RGBLEDs, RGBLED_COUNT);
  }
  sleeptimer.stop();
}

void initSleeptimer() {
  for(uint8_t x = 0; x < 2; x++) { // blink twice
    smoothFade(RGBLEDs, RGBLED_COUNT, u8 255, u8 255, u8 255);
    delay(200);
    smoothFade(RGBLEDs, RGBLED_COUNT, u8 0, u8 0, u8 0);
    delay(200);
  }
  patterns[currentPattern]->setup(); // go back to current pattern

  Serial.println("sleeptimer will wait " + String(SLEEPTIMER) + "s");
  sleeptimer.start();
}
void loop() {
  if(loopTimer.read() % 100 == 0) {
    if(digitalRead(PWR_BUTTON_PIN)) {
      Serial.println("PWR BUTTON DOWN");
      unsigned long start = millis();
      while(digitalRead(PWR_BUTTON_PIN)) { // wait for button release
        unsigned long duration = millis() - start;
        if(isOn && duration > 2000) {  // 2 second delay to turn off
          togglePwr();
          while(digitalRead(PWR_BUTTON_PIN));
          Serial.println("PWR BUTTON UP -> TOGGLE OFF");
          return;
        }
        if(!isOn) {                   // no delay to turn on
          togglePwr();
          while(digitalRead(PWR_BUTTON_PIN));
          Serial.println("PWR BUTTON UP -> TOGGLE ON");
          return;
        }
        delay(50);
      }
      Serial.println("PWR BUTTON UP -> SLEEPTIMER");

      initSleeptimer();
    }
    if(!isOn) {
      delay(300);
      return;
    }
    if(sleeptimer.read() / 1000 >= SLEEPTIMER && sleeptimer.state() == RUNNING) {
      togglePwr();
      return;
    }

    if(digitalRead(FUNCTION_BUTTON_PIN)) {
      Serial.println("FUNCTION BUTTON DOWN");
      currentPattern = (currentPattern + 1) % PATTERN_COUNT;
      patterns[currentPattern]->setup();
      Serial.println("switched to Pattern " + String(currentPattern));
      while(digitalRead(FUNCTION_BUTTON_PIN));
      Serial.println("FUNCTION BUTTON UP");
    }

    patterns[currentPattern]->tick();

    if(Serial.available() && Serial.read() == 'p') {
      int next = Serial.read();
      if(!isDigit(next)) {
        Serial.println("expected (int) after p");
        return;
      }
      long pattern = String((char) next).toInt();
      if(pattern >= 0 && pattern < PATTERN_COUNT) {
        currentPattern = pattern;
        patterns[currentPattern]->setup();
        Serial.println("switched to Pattern " + String(currentPattern));
      } else {
        Serial.println("invalid pattern " + String(pattern));
      }
    }
  }
  rgbled_tick();
}
