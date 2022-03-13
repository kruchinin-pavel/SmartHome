void setup() {
  // put your setup code here, to run once:
  for (int pin = 2; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int pin = 2; pin <= 10; pin++) {
    digitalWrite(pin, HIGH);
  }
  delay(250);
  for (int pin = 2; pin <= 10; pin++) {
    digitalWrite(pin, LOW);
  }
  delay(250);
}
