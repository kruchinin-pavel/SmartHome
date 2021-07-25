#ifndef  func_h
#define func_h 1


int blinkPin(int pin, int highMs, int lowMs) {
  digitalWrite(pin, HIGH);
  delay(highMs);
  digitalWrite(pin, LOW);
  if (lowMs > 0)   delay(lowMs);
  return highMs + lowMs;
}

double analogThrustedRead(int pin) {
  int count = 50;
  double value = 0;
  for (int i = 0; i < count; i++) {
    value += analogRead_(pin);
    delay(10);
  }
  return (value / count);
}


class NormalizedAnalogVal {
    int pin;
    String pinStr, toStringVal;
    int sens = -1, sensMax = 600, sensMin = 400;
  public:
    NormalizedAnalogVal() {}

    NormalizedAnalogVal(int pin, String pinStr, double maxVal, double minVal) {
      this->pin = pin;
      this->pinStr = pinStr;
      this->sensMax = maxVal;
      this->sensMin = minVal;
    };

    int getVal() {
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
    int nomalize(double sens) {
      return (int)sens;
    };
};

enum DispensingState {
  Dispensing = 0, NonDispensing = 1
};

class Dispenser  {
  private:
    DispensingState mode = NonDispensing;
  public:
    Dispenser(const int id, const int rheoPin, const String rheoPinStr, const int sensPin, const String sensPinStr, const int ledPin) {
      this->id = id;
      this->ledPin = ledPin;
      sens = NormalizedAnalogVal(sensPin, sensPinStr, 700., 400.);
      rheo = NormalizedAnalogVal(rheoPin, rheoPinStr, 1024, 0);
      to_string.reserve(128);
    };

    void init() {
      _init();
    }


    virtual int doDispense() {};
    virtual int stopDispense() {};

    virtual int update() {
      rheo.readVal();
      sens.readVal();
      int ret = 0;
      switch (mode) {
        case Dispensing:
          ret += blinkPin(ledPin, 500, 500);
          ret += doDispense();
          if (sens.getVal() < rheo.getVal() - 25) {
            mode = NonDispensing;
          }
          break;
        case NonDispensing:
          ret += blinkPin(ledPin, 10, 900);
          ret += stopDispense();
          if (sens.getVal() > rheo.getVal() + 25) {
            mode = Dispensing;
          }
          break;
      }
      return ret;
    };


    String toString() {
      to_string = "D: " ;
      to_string = id;
      switch (mode) {
        case Dispensing:
          to_string = "Disp";
          break;
        case NonDispensing:
          to_string = "!Disp";
          break;

      }
      to_string += id;
      to_string += " s" + sens.toString();
      to_string += ", r";
      to_string += rheo.toString();
      to_string += "; \t";
      return to_string;
    }



  protected:
    int id, ledPin;
    String to_string;
    NormalizedAnalogVal  rheo, sens;
    virtual void _init() {
      Serial.println("Dispenser init");
      pinMode(ledPin, OUTPUT);
      digitalWrite(ledPin, LOW);
      rheo.init();
      sens.init();
    };
    void blink(int highMs, int lowMs, int count ) {
      for (int i = 0; i < count; i++) blinkPin(ledPin, highMs, lowMs);
    }

};


class CraneDriver {
    int pin1, pin2;
    int is_open = -1;
    String str;
  public:
    CraneDriver(int pin1, int pin2) {
      this->pin1 = pin1;
      this->pin2 = pin2;
      str.reserve(128);
    }

    void do_close() {
      if (is_open != 1) {
        is_open = 1;
        Serial.println("Closing crane");
        switch_crane(pin1, pin2, LOW, HIGH);
      }
    }

    void do_open() {
      if (is_open != 0) {
        is_open = 0;
        Serial.println("Open crane");
        switch_crane(pin1, pin2, HIGH, LOW);
      }
    }

  protected:
    virtual void _init() {
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      switch_crane(pin1, pin2, LOW, LOW);
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

};

class CraneDispenser : public Dispenser, CraneDriver {
  private:
    int nextSwitch = -1;
  public:
    CraneDispenser(int id, int rheoPin, String rheoPinStr, int sensPin, String sensPinStr, int ledPin, int driverPin1, int  driverPin2):
      Dispenser(id, rheoPin, rheoPinStr, sensPin, sensPinStr, ledPin),
      CraneDriver(driverPin1, driverPin2) {
    }


  protected:
    virtual void _init() override {
      Dispenser::_init();
      CraneDriver::_init();
    }


    int doDispense() {
      CraneDriver::do_open();
      return 0;
    }
    int stopDispense() {
      CraneDriver::do_close();
      return 0;
    }

};


class PumpDispenser: public Dispenser {
    int nextPump, pumpPin;
    int dispenseCounter = 0;
  public:
    PumpDispenser(int id, int rheoPin, String rheoPinStr, int sensPin, String sensPinStr, int ledPin, int pumpPin):
      Dispenser(id, rheoPin, rheoPinStr, sensPin, sensPinStr, ledPin) {
      this->pumpPin = pumpPin;
    };

    void init() {
      Dispenser::init();
      pinMode(pumpPin, OUTPUT);
      digitalWrite(pumpPin, LOW);
    }

    int doDispense() {
      int ret = 0;
      if (nextPump-- <= 0) {
        ret += blinkPin(pumpPin, 4500, 0);
        dispenseCounter += 1;
        nextPump = 30 * 60;
      }
      return ret;
    }

    int stopDispense() {
      nextPump = 3;
      return 0;
    }

};


#endif
