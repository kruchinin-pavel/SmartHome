#define LED_PIN 1
// 2-4
#define RHEOSTAT_PIN 2
#define SENS_PIN A0
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

int nextConfCheck = 0;
int nextPumpCheck = 0;
int nextDrynessCheck = 0;
int nextBlink = 0;

double lastDrynessLevel = 0.;

void fetchPump() {
  nextPumpCheck = localTime + 2 * 60 * 1000;
}


void fetchBlink() {
  nextBlink = localTime + 1000;
}

void fetchDryness() {
  nextDrynessCheck = localTime + 5000;
  fetchBlink();
}


void fetchConf() {
  nextConfCheck = localTime + 1000;
  fetchPump();
  fetchDryness();
}

long lastLocalTime = 100;

void loop() {
  if (lastLocalTime > localTime) {
    fetchConf();
  }
  double drynessLevel = getDrynessLevel(RHEOSTAT_PIN);
  if (abs(drynessLevel - lastDrynessLevel) > 10) {
    digitalWrite(LED_PIN, HIGH);
    fetchConf();
  }

  if (nextConfCheck > 0) {
    if (localTime > nextConfCheck) {
      blink(100, 100, 3);
      for (int i = 0; i < 3; i++) blinkPin(LED_PIN, 100, 100);
      nextConfCheck = 0;
      fetchDryness();
    }
  } else if (localTime > nextDrynessCheck) {
    fetchDryness();
    double dryness = analogThrustedRead(SENS_PIN);
    //    showVal(dryness);
    if (needPump(SENS_PIN, drynessLevel)) {
      if (localTime > nextPumpCheck) {
        pumpWater(PUMP_PIN, 3);
        fetchPump();
      } else {
        pumpWater(LED_PIN, 3);
      }
    } else {
      delay_(1000);
    }
  } else if (localTime > nextBlink) {
    fetchBlink();
    blink();
  }
  delay_(100);
  lastDrynessLevel = drynessLevel;
  lastLocalTime = localTime;
}
