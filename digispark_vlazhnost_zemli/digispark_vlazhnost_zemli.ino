#define LED_PIN 13
// 2-4
#define RHEOSTAT_PIN A2
#define SENS_PIN A0
#define PUMP_PIN 2

//#include "digispark.h"
#include "arduino.h"
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

int nextPump = 60;

void loop_test() {
  double drynessLevel = analogThrustedRead(RHEOSTAT_PIN);
  double dryness = analogThrustedRead(SENS_PIN);
  delay(10);
  Serial.println(dryness);
}

void loop_work() {
  double drynessLevel = analogThrustedRead(RHEOSTAT_PIN)+100;
  double dryness = (analogThrustedRead(SENS_PIN)-300)*1024/450;
  String v;
  if (dryness < drynessLevel) {
    blink(100, 100, 3);
    nextPump = 30;
    delay(500);
    v = " ---- ";
  } else {
    if (nextPump-- <= 0) {
      blinkPin(PUMP_PIN, 3000, 0);
      nextPump = 30 * 60;
    }
    blink(1000, 100, 1);
    v = " Lyem ";
  }
#ifdef ARDUINO  
  Serial.print(dryness);
  Serial.print(v);
  Serial.println(drynessLevel);
#endif
}

void loop() {
  loop_work();
}
