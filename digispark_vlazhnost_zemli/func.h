#ifndef func_h
#define func_h 1

void blinkPin(int pin, int highMs, int lowMs) {
  digitalWrite(pin, HIGH);
  delay(highMs);
  digitalWrite(pin, LOW);
  delay(lowMs);
}

void blink(int highMs, int lowMs, int count) {
  for (int i = 0; i < count; i++) blinkPin(LED_PIN, highMs, lowMs);
}

double analogThrustedRead(int pin) {
  int count = 10;
  double value = 0;
  for (int i = 0; i < count; i++) {
    value += analogRead_(pin);
    delay(10);
  }
  return value / count;
}

#endif
