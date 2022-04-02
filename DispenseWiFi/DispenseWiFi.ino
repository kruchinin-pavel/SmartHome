#include "esp8266.h"
#include "handleDisp.h"
#include "func.h"
#include "wifi.h"

void setup() {
  setup_wifi();
  initDispenser();
}

void loop() {
  loop_wifi();
  disp.update();
}
