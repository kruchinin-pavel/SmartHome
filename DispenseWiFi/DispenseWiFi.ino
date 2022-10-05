#include "esp8266.h"
#include "handleDisp.h"
#include "func.h"
#include "wifi.h"


void setup() {
  delay(1000);
  Serial.begin(115200);
  setup_wifi();
  initDispenser();
  pinMode(A0, INPUT);
}

void loop() {
  loop_wifi();
  int tot_delay = 3000;
  tot_delay -= disp.update();
  if (tot_delay > 0) {
    delay(tot_delay);
  }
  check_sleep();
}
