#ifndef func_h
#define func_h 1

int blinkPin(int pin, int highMs, int lowMs) {
  digitalWrite(pin, HIGH);
  delay(highMs);
  digitalWrite(pin, LOW);
  if (lowMs > 0)   delay(lowMs);
  return highMs + lowMs;
}

double analogThrustedRead(int pin) {
  int count = 10;
  double value = 0;
  for (int i = 0; i < count; i++) {
    value += analogRead_(pin);
    delay(10);
  }
  return (int)(value / count);
}


class NormalizedAnalogVal {
    int pin;
    String pinStr, toStringVal;
    double sens = -1, sensMax = 600, sensMin = 400;
    double currToCompare = -1;
  public:
    NormalizedAnalogVal() {}

    NormalizedAnalogVal(int pin, String pinStr, double maxVal, double minVal) {
      this->pin = pin;
      this->pinStr = pinStr;
      this->sensMax = maxVal;
      this->sensMin = minVal;
    };

    boolean lt(double level) {
      if (getVal() - currToCompare > 50) {
        currToCompare = -1;
      }
      double cmp = currToCompare == -1 ? getVal() : currToCompare;
      if (cmp <= level) {
        if (currToCompare == -1 || cmp > getVal() ) {
          currToCompare = cmp - 25;
        }
        return true;
      }
      return false;
    }

    double getVal() {
      return sens;
    }

    double getCmpVal() {
      return currToCompare ;
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
      toStringVal += "(";
      toStringVal += currToCompare;
      toStringVal += ")";
      return toStringVal;
    }

  private:
    int nomalize(double sens) {
      return sens;
      //      if (sens > 0) {
      //        if (sens < sensMin) sensMin = sens;
      //        if (sens > sensMax) sensMax = sens;
      //      }
      //      if (sens > sensMax) {
      //        return sensMax;
      //      }
      //      if (sens < sensMin) {
      //        return sensMin;
      //      }
      //      return (sens - sensMin) * 1024 / (sensMax - sensMin + 1);
    };
};

class Dispenser {
    NormalizedAnalogVal  rheo;
    NormalizedAnalogVal  sens;
    int id, nextPump, pumpPin, ledPin;
    String res;
    String v;
  public:
    Dispenser(int id, int rheoPin, String rheoPinStr, int sensPin, String sensPinStr, int pumpPin, int ledPin) {
      this->id = id;
      sens = NormalizedAnalogVal(sensPin, sensPinStr, 700., 400.);
      rheo = NormalizedAnalogVal(rheoPin, rheoPinStr, 1024, 0);
      this->pumpPin = pumpPin;
      this->ledPin = ledPin;
    };

    void init() {
      pinMode(pumpPin, OUTPUT);
      digitalWrite(pumpPin, LOW);
      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
      rheo.init();
      sens.init();
    }

    int update() {
      rheo.readVal();
      sens.readVal();
      int ret = 0;
      if (rheo.lt(sens.getVal())) {
        ret = blinkPin(ledPin, 1000, 0);
        if (nextPump-- <= 0) {
          ret += blinkPin(pumpPin, 3000, 0);
          nextPump = 30 * 60;
          v = " pmp ";
        } else {
          v = " w";
          v += nextPump;
        }
        v += " ";
      } else {
        ret = blinkPin(ledPin, 10, 0);
        nextPump = 3;
        v = " oki ";
      }
      //      Serial.println(toString());
      return ret;
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


#endif
