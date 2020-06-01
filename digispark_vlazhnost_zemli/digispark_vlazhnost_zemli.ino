//#define ARDUINO
#define DIGISPARK

#ifdef ARDUINO
#define RHEOSTAT_PIN A1
#define RHEOSTAT_PIN_LOG RHEOSTAT_PIN
#define LED_PIN 13
#define SENS_PIN A0
#define SENS_PIN_LOG SENS_PIN
#define ANALOG_RATE 1
#define PUMP_PIN 2
#endif
#ifdef DIGISPARK
#define RHEOSTAT_PIN 13
#define RHEOSTAT_PIN_LOG 13
#define LED_PIN 1
#define SENS_PIN 4
#define SENS_PIN_LOG 2
#define ANALOG_RATE 2
#define PUMP_PIN 0
#endif

long count = 1;

void setup()
{
#ifdef ARDUINO
  Serial.begin(9600);
#endif
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENS_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

void blink() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(300);
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
  delay(durationSeconds * 1000);
  digitalWrite(pumpPin, LOW);
  digitalWrite(LED_PIN, LOW);
}

double analogThrustedRead(int pin) {
  int count = 100;
  double value = 0;
  for (int i = 0; i < count; i++) {
    value += analogRead(pin);
    delay(10);
  }
  return value / count * ANALOG_RATE;
}

void checkAndDispense(int sensorPin, int pumpPin, double drynessLevel) {
  double dryness = analogThrustedRead(sensorPin);
  showVal(dryness);
  if (dryness > drynessLevel) {
#ifdef ARDUINO
    Serial.print("(pump)dryness : ");
    Serial.print(dryness);
    Serial.print(" > drynessLevel: ");
    Serial.println(drynessLevel);
#endif
    pumpWater(pumpPin, 3);
  } else {
#ifdef ARDUINO
    Serial.print("(nothing do)dryness : ");
    Serial.print(dryness);
    Serial.print(" < drynessLevel: ");
    Serial.println(drynessLevel);
#endif
  }
}

void loop() {
  if (count++ % 5 == 0) {
    double drynessLevel = 600.;
#ifdef RHEOSTAT_PIN_LOG
    drynessLevel = analogThrustedRead(RHEOSTAT_PIN_LOG);
#endif
#ifdef ARDUINO
    Serial.print("Rheo: ");
    Serial.println(drynessLevel);
#endif
    checkAndDispense(SENS_PIN_LOG, PUMP_PIN, drynessLevel);
  } else {
    blink();
  }
  if (count > 1000000) count = 0;
  delay(1000);
}
