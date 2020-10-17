#define LED_PIN 13
// 2-4
#define RHEOSTAT_PIN A2
#define SENS_PIN A0
#define PUMP_PIN 2

//#include "digispark.h"
#include "arduino.h"
#include "func.h"

struct Dispenser {
  int nextPump = 60, rheoPin, sensPin, pumpPin;
};
//.rheoPin = A2, .sensPin = A0, .pumpPin = 2
struct Dispenser dispensers[1];

void setup()
{
#ifdef ARDUINO
  Serial.begin(9600);
#endif
  for (int i = 0; i <= 6; i++) pinMode(i, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

void loop_test() {
  double drynessLevel = analogThrustedRead(RHEOSTAT_PIN);
  double dryness = analogThrustedRead(SENS_PIN);
  delay(10);
  Serial.println(dryness);
}

void loop_work(struct Dispenser d) {
  double drynessLevel = analogThrustedRead(d.rheoPin) + 100;
  double dryness = (analogThrustedRead(d.sensPin) - 300) * 1024 / 450;
  String v;
  if (dryness < drynessLevel) {
    blink(100, 100, 3);
    d.nextPump = 30;
    delay(500);
    v = " ---- ";
  } else {
    if (d.nextPump-- <= 0) {
      blinkPin(d.pumpPin, 3000, 0);
      d.nextPump = 30 * 60;
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
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
  for (int i = 0; i < n; i++) {
    loop_work(dispensers[i]);
  }
}
