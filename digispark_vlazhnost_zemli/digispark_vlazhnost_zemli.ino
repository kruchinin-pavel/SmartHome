#define LED_PIN 0
//#include "digispark.h"
#include "arduino.h"
#include "func.h"

class NormalizedAnalogVal {
    int pin;
    String pinStr, toStringVal;
    double sens = -1, sensMax = 600, sensMin = 400;
  public:
    NormalizedAnalogVal() {}
    
    NormalizedAnalogVal(int pin, String pinStr, double maxVal, double minVal) {
      this->pin = pin;
      this->pinStr = pinStr;
      this->sensMax = maxVal;
      this->sensMin = minVal;
    };

    double getVal() {
      return sens;
    }

    void readVal() {
      if (enabled()) {
        sens = nomalize(analogThrustedRead(pin));
      }
    };

    boolean enabled() {
      return pin > 0;
    }

    void init() {
      pinMode(pin, INPUT);
      Serial.print(pinStr);
      Serial.println(" made INPUT");
    }

    String toString() {
      toStringVal = pinStr;
      toStringVal += "=";
      toStringVal += sens;
      return toStringVal;
    }

  private:
    double nomalize(double sens) {
      if (sens > 0) {
        if (sens < sensMin) sensMin = sens;
        if (sens > sensMax) sensMax = sens;
      }
      if (sens > sensMax ||  sens < sensMin)  return -1;
      return (sens - sensMin) * 1024 / (sensMax - sensMin + 1);
    };
};

class Dispenser {
    NormalizedAnalogVal  rheo;
    NormalizedAnalogVal  sens;
    int id, nextPump, pumpPin, ledPin;
    String res;
    String v;
  public:
    Dispenser(int id, int rheoPin, int sensPin, String sensPinStr, int pumpPin, int ledPin) {
      this->id = id;
      sens = NormalizedAnalogVal(sensPin, sensPinStr, 700., 400.);
      rheo = NormalizedAnalogVal(rheoPin, "", 1024, 0);
      this->pumpPin = pumpPin;
      this->ledPin = ledPin;
    };

    void init() {
      rheo.init();
      sens.init();
      pinMode(pumpPin, OUTPUT);
      pinMode(ledPin, OUTPUT);
    }

    void update() {
      rheo.readVal();
      sens.readVal();
      if (sens.getVal() > rheo.getVal()) {
        digitalWrite(ledPin, HIGH);
        if (nextPump-- <= 0) {
          //          blinkPin(pumpPin, 3000, 0);
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
      //      Serial.println(toString());
    };

    String toString() {
      String res = "";
      res += id;
      res += " s" + sens.toString();
      if (rheo.enabled()) {
        res += v;
        res += "r";
        res += rheo.toString();
      }
      res += ";\t";
      this->res = res;
      return this->res;
    }

  private:
    void blink(int highMs, int lowMs, int count) {
      for (int i = 0; i < count; i++) blinkPin(ledPin, highMs, lowMs);
    }

};

Dispenser dispensers[] = {
  Dispenser( 1, -1, A3, "A3", 5, 2),
  Dispenser( 2, -1, A5, "A5", 6, 3),
  Dispenser( 3, -1, A7, "A7", 7, 4)
};
size_t n;

void setup()
{
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
#ifdef ARDUINO
  Serial.begin(9600);
#endif
  for (int i = 0; i < n; i++) dispensers[i].init();
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
