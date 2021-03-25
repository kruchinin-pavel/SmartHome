#define LED_PIN 0
//#include "digispark.h"
#include "arduino.h"
#include "func.h"

//(id, rheoPin, rheoPinStr, sensPin, sensPinStr, pumpPin, ledPin) {
Dispenser dispensers[] = {
  Dispenser( 1, A5, "A5", A4, "A4", 7, 2), //Blue (left)
  Dispenser( 2, A7, "A7", A3, "A3", 6, 3), //Yellow (Middle)
  Dispenser( 3, A6, "A6", A2, "A2", 5, 4)  //Red (right)
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
