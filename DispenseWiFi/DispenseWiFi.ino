#include "esp8266.h"
#include "handleDisp.h"
#include "func.h"
#include "wifi.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Ждем, пока все загрузится.
  while (!Serial) { }
  delay(2000);
  Serial.println("Started");
  setup_wifi();
  initDispenser();
  pinMode(A0, INPUT);
}

void loop() {
  loop_wifi();
  int tot_delay = 5000;
  tot_delay -= disp.update();
  if (tot_delay > 0) {
    delay(tot_delay);
  }
  // report current level
  report(disp.getCurrent(), disp.getReoMin(), disp.getReoMax());
}
