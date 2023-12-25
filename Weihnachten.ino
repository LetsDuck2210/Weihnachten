#include "rgbled.h"
#include "pattern.h"
#include "colorfade.h"
#include "syncfade.h"
#include "staticpattern.h"

#define BUTTON_PIN 2
#define RGBLED_COUNT 3
RGBLed RGBLEDs[RGBLED_COUNT] = {
  { 3, 5, 6,    255, 0, 0 },
  { 11, 10, 9,  0, 255, 0 },
  { A0, A1, A2, 0, 0, 255 }
};
bool isOn = true;

#define PATTERN_COUNT 3
Pattern* patterns[PATTERN_COUNT] = {
  new ColorFade(RGBLEDs, RGBLED_COUNT),
  new SyncFade(RGBLEDs, RGBLED_COUNT),
  new StaticPattern(RGBLEDs, RGBLED_COUNT)
};
int currentPattern = 0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN, INPUT);
  for (int i = 0; i < RGBLED_COUNT; i++) {
    pinMode(RGBLEDs[i].pinR, OUTPUT);
    pinMode(RGBLEDs[i].pinG, OUTPUT);
    pinMode(RGBLEDs[i].pinB, OUTPUT);
  }
  Serial.println("initialized all leds as output");
  Serial.println("using Pattern 0");

  patterns[currentPattern]->setup();
}

void togglePwr() {
  isOn = !isOn;
  if(!isOn) {
    smoothFade(RGBLEDs, RGBLED_COUNT, (uint8_t) 0, (uint8_t) 0, (uint8_t) 0);
  } else {
    setDefaults(RGBLEDs, RGBLED_COUNT);
  }
}
void loop() {
  if(digitalRead(BUTTON_PIN)) {
    unsigned long start = millis();
    while(digitalRead(BUTTON_PIN)) { // wait for button release
      unsigned long duration = millis() - start;
      if(isOn && duration > 2000) {  // 2 second delay to turn off
        togglePwr();
        while(digitalRead(BUTTON_PIN));
        return;
      }
      if(!isOn) {                   // no delay to turn on
        togglePwr();
        while(digitalRead(BUTTON_PIN));
        return;
      }
      delay(50);
    } 
    
    currentPattern = (currentPattern + 1) % PATTERN_COUNT;
    patterns[currentPattern]->setup();
    Serial.println("switched to Pattern " + String(currentPattern));
  }
  if(!isOn) {
    delay(500);
    return;
  }

  for(int i = 0; i < RGBLED_COUNT; i++) {
    patterns[currentPattern]->tick();
  }
  delay(25);
}
