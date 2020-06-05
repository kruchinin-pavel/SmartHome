#define LED_PIN 1
// 2-4
#define RHEOSTAT_PIN 2
#define SENS_PIN 3
#define PUMP_PIN 4

#include "digispark.h"
//#include "arduino.h"
#include "func.h"

void setup()
{
#ifdef ARDUINO
  Serial.begin(9600);
#endif
  for (int i = 0; i <= 6; i++) pinMode(i, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

int nextPump = 0;

void loop() {
  double drynessLevel = analogThrustedRead(RHEOSTAT_PIN);
  double dryness = analogThrustedRead(SENS_PIN);
  if (dryness < drynessLevel) {
    blink(100, 100, 3);
    nextPump = 30;
    delay(500);
  } else {
    if (nextPump-- <= 0) {
      blinkPin(PUMP_PIN, 3000, 0);
      nextPump = 30*60;
    }
    blink(1000, 100, 1);
  }
  
}
