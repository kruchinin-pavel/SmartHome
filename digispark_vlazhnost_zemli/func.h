#ifndef func_h
#define func_h 1
long localTime = 0;

void delay_(int ms){
  localTime += ms;
  delay(ms);
  if (localTime> 10000000) localTime = 0;
}

void blinkPin(int pin, int highMs, int lowMs) {
  digitalWrite(pin, HIGH);
  delay_(highMs);
  digitalWrite(pin, LOW);
  delay_(lowMs);
}

void blink(int highMs, int lowMs, int count) {
  for(int i=0;i<count;i++) blinkPin(LED_PIN, highMs, lowMs);
}
void blink() {
  blinkPin(LED_PIN, 100, 200);
}

void showVal(double value) {
#ifdef ARDUINO
  Serial.println(value);
#endif
  for (int i = 0; i < value / 50; i++) {
    blink();
  }
}

void pumpWater(int pumpPin, int durationSeconds) {
  digitalWrite(pumpPin, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay_(durationSeconds * 1000);
  digitalWrite(pumpPin, LOW);
  digitalWrite(LED_PIN, LOW);
}


double analogThrustedRead(int pin) {
  int count = 10;
  double value = 0;
  for (int i = 0; i < count; i++) {
    value += analogRead_(pin);
    delay_(10);
  }
  return value / count;
}


boolean needPump(int sensorPin, double drynessLevel) {
  double dryness = analogThrustedRead(sensorPin);
  showVal(dryness);
  if (dryness > drynessLevel) {
#ifdef ARDUINO
    Serial.print("(pump)dryness : ");
    Serial.print(dryness);
    Serial.print(" > drynessLevel: ");
    Serial.println(drynessLevel);
#endif
    return true;
#ifdef ARDUINO
  } else {
    Serial.print("(nothing do)dryness : ");
    Serial.print(dryness);
    Serial.print(" < drynessLevel: ");
    Serial.println(drynessLevel);
#endif
  }
  return false;
}

double getDrynessLevel(int pin) {
  double drynessLevel = analogThrustedRead(pin);
#ifdef ARDUINO
  Serial.print("Rheo: ");
  Serial.println(drynessLevel);
#endif
  return drynessLevel;
}

#endif
