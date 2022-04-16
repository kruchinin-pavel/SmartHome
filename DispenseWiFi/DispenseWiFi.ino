//#include "esp8266.h"
//#include "handleDisp.h"
//#include "func.h"
//#include "wifi.h"

void setup() {
//  setup_wifi();
//  initDispenser();
  pinMode(A0, INPUT);
}

void loop() {
//  loop_wifi();
  int tot_delay = 3000;
//  tot_delay -= disp.update();
  double v = analogRead(A0);
  String s{"A0 read "};
  s += v;
  Serial.println(s);
  if (tot_delay > 0) {
    delay(tot_delay);
  }

}
