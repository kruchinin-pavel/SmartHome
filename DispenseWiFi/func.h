#if not defined(FUNC_H)
#define FUNC_H 1

struct Output {
    Output(int pin): Output{pin, false} {};
    Output(int pin, bool inversed): inversed{inversed}, pin{pin} {
      pinMode(pin, OUTPUT);
    };
    int blink(int ms) const {
      digitalWrite(pin, (inversed ? LOW : HIGH));
      delay(ms);
      digitalWrite(pin, (inversed ? HIGH : LOW));
      return ms;
    };
  private:
    const bool inversed;
    const int pin;
};


struct Input {
    Input(): Input{ -1, "", 0} {

    }
    Input(int pin, String pinStr, int treshold): pin{pin}, pinStr{pinStr}, treshold{treshold} {
      pinMode(pin, INPUT);
      Serial.print(pinStr);
      Serial.println(" made INPUT");
    };

    bool lower_than(double than) {
      int val = getVal();
      int cmpVal = than + (lastLowerThan ? treshold : - treshold);
      if (cmpVal < 0 || cmpVal > 1000) cmpVal = than;
      if (val > cmpVal) {
        lastLowerThan = false;
      } if (val < cmpVal) {
        lastLowerThan = true;
      }
      return lastLowerThan;
    }

    double getVal() const {
      return sens;
    }

    void readVal() {
      if (enabled()) {
        sens = nomalize(analogThrustedRead());
      }
    };

    String toString() {
      toStringVal = pinStr;
      toStringVal += "=";
      toStringVal += sens;
      toStringVal += "(lLT ";
      toStringVal += lastLowerThan;
      toStringVal += ")";
      return toStringVal;
    }

  private:
    boolean enabled() const {
      return pin > 0;
    }


    int nomalize(double sens) {
      return sens;
    };

    double analogThrustedRead() {
      int count = 10;
      double value = 0;
      for (int i = 0; i < count; i++) {
        value += analogRead_(pin);
        delay(10);
      }
      return (int)(value / count);
    }
    const int pin, treshold;
    const String pinStr;
    String toStringVal;
    double sens = -1;
    bool lastLowerThan = false;
};

struct ExtRegulatedDispenser {
    ExtRegulatedDispenser(int id, int sensPin, String sensPinStr, int pumpPin, int ledPin):
      reoVal{1}, ledPin{ledPin, true}, id{id}, pumpPin{pumpPin}, sens{sensPin, sensPinStr, 13} {
    };

    void setReo(int val) {
      this->reoVal = val;
    }

    int getReo() const {
      return this->reoVal;
    }

    int update() {
      sens.readVal();
      int ret = 0;
      if (sens.lower_than(reoVal)) {
        ret += ledPin.blink(10);
        nextPump = 3;
        v = " oki ";
      } else {
        ret += ledPin.blink(1000);
        if (nextPump-- <= 0) {
          ret += pumpPin.blink(3000);
          nextPump = 30 * 60;
          v = " pmp ";
        } else {
          v = " w";
          v += nextPump;
        }
        v += " ";
      }
      Serial.println(toString());
      return ret;
    };

    String toString() {
      String res = "";
      res += id;
      res += " s" + sens.toString();
      res += v;
      res += "r";
      res += reoVal;
      res += ";\t";
      this->res = res;
      return this->res;
    }
  private:
    Input  sens;
    const int id;
    const Output ledPin, pumpPin;
    int nextPump, reoVal;
    String res, v;
};



#endif
