typedef struct {
  uint8_t pinR, pinG, pinB;
  uint8_t valR, valG, valB;
  uint8_t requiredOffset, currentOffset;
} RGBLed;
typedef struct {
  uint8_t pin, value;
} LED;

#define BUTTON_PIN 2
#define RGBLED_COUNT 3
RGBLed RGBLEDs[RGBLED_COUNT] = {
  { 3, 5, 6,    255, 0, 0,    0, 0 },
  { 9, 10, 11,  0, 255, 0,    0, 0 },
  { A0, A1, A2, 0, 0, 255,    1, 0 }
};
bool isOn = true;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN, INPUT);
  for (int i = 0; i < RGBLED_COUNT; i++) {
    pinMode(RGBLEDs[i].pinR, OUTPUT);
    pinMode(RGBLEDs[i].pinG, OUTPUT);
    pinMode(RGBLEDs[i].pinB, OUTPUT);
    RGBLEDs[i].requiredOffset += random(4);
    Serial.println("initialized RGB (" + String(RGBLEDs[i].pinR) + ") with offset " + String(RGBLEDs[i].requiredOffset));
  }
}

void writeRGB(RGBLed led) {
  analogWrite(led.pinR, led.valR);
  analogWrite(led.pinG, led.valG);
  analogWrite(led.pinB, led.valB);
}
void shift(RGBLed* led) {
  if(led->currentOffset < led->requiredOffset) { // only process every requiredOffset-th shift
    led->currentOffset++;
    return;
  }
  led->currentOffset = 0;

  if(led->valR < 255 && led->valG == 0 && led->valB == 0)   // (all zero -> fade in red)
    led->valR++;
  if(led->valR == 255 && led->valG < 255 && led->valB == 0) // red on -> fade in green
    led->valG++;
  if(led->valR > 0 && led->valG == 255 && led->valB == 0)   // green on -> fade out red
    led->valR--;
  if(led->valR == 0 && led->valG == 255 && led->valB < 255) // green on -> fade in blue
    led->valB++;
  if(led->valR == 0 && led->valG > 0 && led->valB == 255)   // blue on -> fade out green
    led->valG--;
  if(led->valR < 255 && led->valG == 0 && led->valB == 255) // blue on -> fade in red
    led->valR++;
  if(led->valR == 255 && led->valG == 0 && led->valB > 0)   // red on -> fade out blue
    led->valB--;

  writeRGB(*led);
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
    togglePwr();
    while(digitalRead(BUTTON_PIN)); // wait for button release
  }
  if(!isOn) {
    delay(500);
    return;
  }

  for(int i = 0; i < RGBLED_COUNT; i++) {
    shift(&RGBLEDs[i]);
  }
  delay(1);
}
