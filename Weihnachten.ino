#include "rgbled.h"
#include "pattern.h"
#include "colorfade.h"
#include "syncfade.h"
#include "staticpattern.h"
#include "ui.h"
#include <Timer.h>

// === Settings === 
#define SLEEPTIMER (3 * 60 * 60)  // 3 hours
#define PWR_BUTTON_PIN 2          // button to control power (on/off/sleeptimer)
#define FUNCTION_BUTTON_PIN 4     // button to control the running program
#define RGBLED_COUNT 3
RGBLed RGBLEDs[RGBLED_COUNT] = {
  { A2, 5, 6,    255, 0, 0 },
  { 11, 10, 9,  0, 255, 0 },
  { A0, 3, A1, 0, 0, 255 }
};
// ================

bool isOn = true;

#define PATTERN_COUNT 3
Pattern* patterns[PATTERN_COUNT] = {
  new ColorFade(RGBLEDs, RGBLED_COUNT),
  new SyncFade(RGBLEDs, RGBLED_COUNT),
  new StaticPattern(RGBLEDs, RGBLED_COUNT)
};
int currentPattern = 0;
Timer loopTimer;

#ifdef SINGLE_BUTTON
UI* ui = new SingleButtonUI(PWR_BUTTON_PIN);
#else
UI* ui = new DoubleButtonUI(PWR_BUTTON_PIN, FUNCTION_BUTTON_PIN);
#endif

#define u8 (uint8_t)

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

void toggleSleeptimer() {
  bool turnOn = sleeptimer.state() != RUNNING;
  for(uint8_t x = 0; x < (turnOn ? 2 : 1); x++) { // blink twice for on, once for off
    smoothFade(RGBLEDs, RGBLED_COUNT, u8 255, u8 255, u8 255);
    delay(200);
    smoothFade(RGBLEDs, RGBLED_COUNT, u8 0, u8 0, u8 0);
    delay(200);
  }
  patterns[currentPattern]->setup(); // go back to current pattern

  if(turnOn) {
    Serial.println("sleeptimer will wait " + String(SLEEPTIMER) + "s");
    sleeptimer.start();
  } else {
    Serial.println("sleeptimer disabled");
    sleeptimer.stop();
  }
}
void setPattern(uint8_t index) {
  if(index >= PATTERN_COUNT) {
    Serial.println("Invalid pattern: " + String(index));
    return;
  }

  currentPattern = index;
  patterns[currentPattern]->setup();
  Serial.println("Switched to pattern " + String(currentPattern));
}
void nextPattern() {
  setPattern((currentPattern + 1) % PATTERN_COUNT);
}
void loop() {
  rgbled_tick();
  if(loopTimer.read() % 25 != 0) 
    return; 

  Action action = ui->tick(isOn);
  switch(action) {
    case NOTHING:
      break;
    case POWER_TOGGLE:
      togglePwr();
      break;
    case NEXT_PATTERN:
      nextPattern();
      break;
    case SLEEPTIMER_TOGGLE:
      toggleSleeptimer();
      break;
  }

  if(!isOn) {
    delay(300);
    return;
  }
  if(sleeptimer.read() / 1000 >= SLEEPTIMER && sleeptimer.state() == RUNNING) {
    togglePwr();
    return;
  }

  patterns[currentPattern]->tick();

  if(Serial.available() && Serial.read() == 'p') {
    int next = Serial.read();
    if(!isDigit(next)) {
      Serial.println("expected (int) after p");
      return;
    }
    uint8_t pattern = String((char) next).toInt();
    setPattern(pattern);
  }
}
