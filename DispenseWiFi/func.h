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
    Input(): Input{ -1, ""} {

    }
    Input(int pin, String pinStr): pin{pin}, pinStr{pinStr} {
      pinMode(pin, INPUT);
      Serial.print(pinStr);
      Serial.println(" made INPUT");
    };


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
    const int pin;
    const String pinStr;
    String toStringVal;
    double sens = -1;
};

struct AbstractDriver {
  virtual int do_open() = 0, do_close() = 0, do_update() = 0;
  virtual String get_state() = 0;
};


struct ExtRegulatedDispenser {
    ExtRegulatedDispenser(int id, int sensPin, String sensPinStr, int ledPin, AbstractDriver *driver):
      reoValMax{1}, reoValMin{1}, ledPin{ledPin, true}, id{id}, sens{sensPin, sensPinStr}, driver{driver} {

    };

    int getCurrent() {
      return sens.getVal();
    }

    void setReo(int val) {
      val = val % 1024;
      this->reoValMax = val;
      this->reoValMin = val;
    }

    void setReoMax(int val) {
      val = val % 1024;
      this->reoValMax = val;
    }

    void setReoMin(int val) {
      val = val % 1024;
      this->reoValMin = val;
    }

    int getReoMax() const {
      return this->reoValMax;
    }

    int getReoMin() const {
      return this->reoValMin;
    }


    int doPump() {
      return driver->do_open();
    }

    int stopPump() {
      Serial.println("stopPump");
      return driver->do_close();
    }

    String getState() const {
      return driver->get_state();
    }

    int update() {
      sens.readVal();
      int ret = 0;
      if (sens.getVal() < reoValMin) {
        ret += ledPin.blink(10);
        Serial.println("Call stop pump");
        ret += stopPump();
      } else if (sens.getVal() > reoValMax) {
        ret += ledPin.blink(1000);
//        ret += doPump();
      }
      v += driver->do_update();
      //Serial.println(toString());
      return ret;
    };

    String toString() {
      String res = "";
      res += id;
      res += " s" + sens.toString();
      res += v;
      res += "r";
      res += this->reoValMin;
      res += "\\";
      res += this->reoValMax;
      res += "; \t";

      this->res = res;
      return this->res;
    }

    ~ExtRegulatedDispenser() {
      delete driver;
    }
  private:
    Input  sens;
    const int id;
    const Output ledPin;
    AbstractDriver *driver;
    int reoValMax, reoValMin;
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

    bool is_pumping() {
      return is_open == 1;
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

    PumpDriver(int pin): pump{pin} {
    }

    virtual int do_close() {
      pumping = false;
    }

    virtual int do_open() {
      
      if (!pumping) {
        pumping = true;
        nextPump = 0;
      }
    }

    virtual int do_update() {
      if (!pumping) {
        return 0;
      }
      int ret;
      if (nextPump-- <= 0) {
        nextPump = 30 * 60;
        ret += pump.blink(3000);
        pumpCount++;
      }
      return ret;
    }

    String get_state() {
      if (pumping) {
        state = String("open(") + pumpCount + ")";
      } else   {
        state = String("closed(") + pumpCount + ")";
      }
      return state;
    }

  private:


    bool is_pumping() {
      return pumping;
    }

    String state;
    const Output pump;
    int nextPump, pumpCount;
    bool pumping;
};

#endif
