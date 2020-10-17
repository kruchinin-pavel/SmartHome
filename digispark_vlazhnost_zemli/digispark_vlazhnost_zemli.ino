#define LED_PIN 13
// 2-4
#define RHEOSTAT_PIN A2
#define SENS_PIN A0
#define PUMP_PIN 2

//#include "digispark.h"
#include "arduino.h"
#include "func.h"
class Dispenser {
    int id, nextPump, rheoPin, sensPin, pumpPin;
    double rheo, sens;
    String res;
    String v;
  public:
    Dispenser(int id, int rheoPin, int sensPin, int pumpPin) {
      this->id = id;
      this->sensPin = sensPin;
      this->rheoPin = rheoPin;
      this->pumpPin = pumpPin;
    };
    void update() {
      double drynessLevel = analogThrustedRead(rheoPin) + 100;
      double dryness = (analogThrustedRead(sensPin) - 300) * 1024 / 450;
      rheo = drynessLevel;
      sens = dryness;
      if (dryness < drynessLevel) {
        nextPump = 30;
        v = " ---- ";
      } else {
        if (nextPump-- <= 0) {
          blinkPin(pumpPin, 3000, 0);
          nextPump = 30 * 60;
        }
        v = " Lyem " + id;
      }
      toString();
    };

    void toString() {
#ifdef ARDUINO
      String res = "";
      res += id;
      res += ": sens ";
      res += sens;
      res += v;
      res += ": rheo ";
      res += rheo;
      res += "\t";
      this->res = res;
#endif
    }

    String getStr() {
      return res;
    }

};

Dispenser dispensers[3] = {
  Dispenser( 1, A2, A3, 5),
  Dispenser( 2, A1, A4, 6),
  Dispenser( 3, A0, A5, 7)
};

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

void loop() {
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
  for (int i = 0; i < n; i++) {
    dispensers[i].update();
  }
  blink(1000, 100, 1);
#ifdef ARDUINO
  Serial.print("ln-> ");
  for (int i = 0; i < n; i++) {
    Serial.print(dispensers[i].getStr());
  }
  Serial.println();
#endif
}
