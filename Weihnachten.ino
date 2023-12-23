#include "rgbled.h"
#include "pattern.h"
#include "colorfade.h"

#define BUTTON_PIN 2
#define RGBLED_COUNT 3
RGBLed RGBLEDs[RGBLED_COUNT] = {
  { 3, 5, 6,    255, 0, 0 },
  { 9, 10, 11,  0, 255, 0 },
  { A0, A1, A2, 0, 0, 255 }
};
bool isOn = true;

#define PATTERN_COUNT 1
Pattern* patterns[PATTERN_COUNT] = {
  new ColorFade(RGBLEDs, RGBLED_COUNT)
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
}

void togglePwr() {
  isOn = !isOn;
  if(!isOn) {
    for(int i = 0; i < RGBLED_COUNT; i++) {
      RGBLEDs[i].valR = RGBLEDs[i].valG = RGBLEDs[i].valB = 0;
      writeRGB(RGBLEDs[i]);
    }
  } else {
    for(int i = 0; i < RGBLED_COUNT; i++) {
      uint8_t r[] = {255, 0, 0},    //  v
              g[] = {0, 255, 0},    //  
              b[] = {0, 0, 255};    //  prevent exact synchronization
      RGBLEDs[i].valR = r[i];       //  
      RGBLEDs[i].valG = g[i];       //  
      RGBLEDs[i].valB = b[i];       //  ^
      writeRGB(RGBLEDs[i]);
    }
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
  }
  if(!isOn) {
    delay(500);
    return;
  }

  for(int i = 0; i < RGBLED_COUNT; i++) {
    patterns[currentPattern]->tick();
  }
  delay(1);
}
