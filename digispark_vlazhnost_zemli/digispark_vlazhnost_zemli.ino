#define LED_PIN 0
//#include "digispark.h"
#include "arduino.h"
#include "func.h"

class Dispenser {
    int id, nextPump, rheoPin, sensPin, pumpPin, ledPin;
    int rheo, sens;
    String res;
    String v;
  public:
    Dispenser(int id, int rheoPin, int sensPin, int pumpPin, int ledPin) {
      this->id = id;
      this->sensPin = sensPin;
      this->rheoPin = rheoPin;
      this->pumpPin = pumpPin;
      this->ledPin = ledPin;
    };

    void init() {
      pinMode(rheoPin, INPUT);
      pinMode(sensPin, INPUT);
      pinMode(pumpPin, OUTPUT);
      pinMode(ledPin, OUTPUT);
    }

    void update() {
      //      double drynessLevel = analogThrustedRead(rheoPin) + 100;
      //      double dryness = (analogThrustedRead(sensPin) - 300) * 1024 / 450;
      rheo = analogRead(rheoPin);
      sens = analogRead(sensPin);
      if (sens > rheo) {
        digitalWrite(ledPin, HIGH);
        if (nextPump-- <= 0) {
          blinkPin(pumpPin, 3000, 0);
          nextPump = 30 * 60;
          v = " pmp ";
        } else {
          v = " w";
          v += nextPump;
        }
        v += " ";
      } else {
        blinkPin(ledPin, 10, 0);
        nextPump = 30;
        v = " oki ";
      }
      toString();
    };

    String toString() {
      String res = "";
      res += "s";
      res += sens;
      res += v;
      res += "r";
      res += rheo;
      res += ";\t";
      this->res = res;
      return res;
    }

  private:
    void blink(int highMs, int lowMs, int count) {
      for (int i = 0; i < count; i++) blinkPin(ledPin, highMs, lowMs);
    }


};

Dispenser dispensers[3] = {
  Dispenser( 1, A2, A3, 5, 2),
  Dispenser( 2, A1, A4, 6, 3),
  Dispenser( 3, A0, A5, 7, 4)
};
size_t n;

void setup()
{
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
  for (int i = 0; i < n; i++) dispensers[i].init();
#ifdef ARDUINO
  Serial.begin(9600);
#endif
}

void loop() {
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
  for (int i = 0; i < n; i++) {
    dispensers[i].update();
  }
  delay(1000);
#ifdef ARDUINO
  for (int i = 0; i < n; i++) {
    Serial.print(dispensers[i].toString());
  }
  Serial.println();
#endif
}
