#define LED_PIN 0
//#include "digispark.h"
#include "arduino.h"
#include "func.h"
#define DEBUG_D

//*****************************************************************
// Полив цветков (помп)
//(id, rheoPin, rheoPinStr, sensPin, sensPinStr, ledPin, pumpPin) {
//Dispenser dispensers[] = {
// Плата макетная
//  PumpDispenser( 1, A5, "A5", A4, "A4", 2, 7), //Blue (left)
//  PumpDispenser( 2, A7, "A7", A3, "A3", 3, 6), //Yellow (Middle)
//  PumpDispenser( 3, A6, "A6", A2, "A2", 4, 5) //Red (right)
//};

// Плата текстолитовая
Dispenser *dispensers[] = {
  new PumpDispenser( 1, A0, "A0", A4, "A4", 6, 7), //Blue (left)
  new PumpDispenser( 2, A7, "A7", A2, "A2", 3, 6), //Green (Middle)
  new PumpDispenser( 3, A6, "A6", A1, "A1", 8, 5) //White (right)
};
//*****************************************************************
// Полив теплицы (кран)
//Dispenser *dispensers[] = {
//  new CraneDispenser( 1, A1, "A1", A4, "A4", 4, 2, 3) //Blue (left)
//};
//*****************************************************************
size_t n;
String msg;

void setup()
{
  msg.reserve(256);
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
#ifdef ARDUINO
  Serial.begin(9600);
#endif
  for (int i = 0; i < n; i++) dispensers[i]->init();
}

void loop() {
  size_t n = sizeof(dispensers) / sizeof(dispensers[0]);
  int totDelay = 0;
  for (int i = 0; i < n; i++) {
    totDelay += dispensers[i]->update();
  }
  if (1000 > totDelay) delay(1000 - totDelay);
#ifdef DEBUG_D
  msg = "";
  for (int i = 0; i < n; i++) {
    msg += dispensers[i]->toString();
  }
  Serial.println(msg);
#endif
}
