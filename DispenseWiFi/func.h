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

struct AbstractDriver {
    virtual int do_open() = 0, do_close() = 0, do_update() = 0;
};


struct ExtRegulatedDispenser {
    ExtRegulatedDispenser(int id, int sensPin, String sensPinStr, int ledPin, AbstractDriver &driver):
      reoValMax{1}, reoValMin{1}, ledPin{ledPin, true}, id{id}, sens{sensPin, sensPinStr, 13}, driver{driver} {

    };

    void setReo(int val) {
      this->reoValMax = val;
      this->reoValMin = val;
    }

    void setReoMax(int val) {
      this->reoValMax = val;
    }

    void setReoMin(int val) {
      this->reoValMin = val;
    }

    int getReoMax() const {
      return this->reoValMax;
    }

    int getReoMin() const {
      return this->reoValMin;
    }


    int doPump() {
      pumpCount++;
      return driver.do_open();
    }

    int getPumpCount() const {
      return pumpCount;
    }

    int update() {
      sens.readVal();
      int ret = 0;
      if (sens.getVal() < reoValMin) {
        ret += ledPin.blink(10);
        nextPump = 3;
        v = " oki ";
      } else if (sens.getVal() > reoValMax) {
        ret += ledPin.blink(1000);
        if (nextPump-- <= 0) {
          ret += doPump();
          nextPump = 30 * 60;
          v = " pmp ";
        } else {
          v = " w";
          v += nextPump;
        }
        v += " ";
      }
      v += this->driver.do_update();
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

    ~ExtRegulatedDispenser() {
      delete this->driver;
    }
  private:
    int pumpCount = 0;
    Input  sens;
    const int id;
    const Output ledPin;
    AbstractDriver &driver;
    int nextPump, reoValMax, reoValMin;
    String res, v;
};


struct CraneDriver : AbstractDriver {
    CraneDriver(int pin1, int pin2) {
      this->pin1 = pin1;
      this->pin2 = pin2;
      str.reserve(128);
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      switch_crane(pin1, pin2, LOW, LOW);
    }

    virtual int do_close() {
      if (is_open != 1) {
        is_open = 1;
        Serial.println("Closing crane");
        switch_crane(pin1, pin2, LOW, HIGH);
      }
    }

    int do_open() {
      if (is_open != 0) {
        is_open = 0;
        Serial.println("Open crane");
        switch_crane(pin1, pin2, HIGH, LOW);
      }
    }

  private:

    void switch_crane(int p1, int p2, int s1, int s2) {
      write_pin(p1, s1);
      write_pin(p2, s2);
    }
    void write_pin(int pin, int s) {
#ifdef DEBUG_D
      String str = "Write: ";
      str += pin;
      str += " = ";
      str += s;
      str += "; ";
      Serial.println(str);
#endif
      digitalWrite(pin, s);
    }

    int pin1, pin2;
    int is_open = -1;
    String str;
};

struct PumpDriver : AbstractDriver {

    CraneDriver(int pin): pump{pin} {
    }

    virtual int do_close() {

    }

    virtual int do_open() {
      return pump.blink(3000);
    }

  private:
    const Output pump;

};

#endif
