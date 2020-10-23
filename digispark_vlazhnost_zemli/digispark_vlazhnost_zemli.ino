#define LED_PIN 0
//#include "digispark.h"
#include "arduino.h"
#include "func.h"

Dispenser dispensers[] = {
  Dispenser( 1, A2, "A2", A5, "A5", 5, 2),
  Dispenser( 2, A3, "A3", A6, "A6", 6, 3),
  Dispenser( 3, A4, "A4", A7, "A7", 7, 4)
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
  int totDelay = 0;
  for (int i = 0; i < n; i++) {
    totDelay += dispensers[i].update();
  }
  if (1000 > totDelay) delay(1000 - totDelay);
#ifdef ARDUINO
  for (int i = 0; i < n; i++) {
    Serial.print(dispensers[i].toString());
  }
  Serial.println();
#endif
}
