int pumpPinL[] = {2};
int pumpPinR[] = {3};

size_t n = sizeof(pumpPinL) / sizeof(int);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < n; i++) {
    pinMode(pumpPinL[i], OUTPUT);
    pinMode(pumpPinR[i], OUTPUT);
  }
  String msg = "Start with ";
  msg += n;
  Serial.println(msg);
  switch_crane(pumpPinL, pumpPinR, LOW, LOW);
}

void pause_crane() {
  String str = "pause: " + switch_crane(pumpPinL, pumpPinR, LOW, LOW);
  Serial.println(str);
  delay(3000);
}

void switchUp() {
  String str = "up: " +  switch_crane(pumpPinL, pumpPinR, HIGH, LOW);
  Serial.println(str);
  delay(60000);
}
void switchDown() {
  String str = "down: " +  switch_crane(pumpPinL, pumpPinR, LOW, HIGH);
  Serial.println(str);
  delay(60000);
}

String write_pin(int pin, int s) {
  String str = "";
  str += pin;
  str += "=";
  str += s;
  str += "; ";
  digitalWrite(pin, s);
  return str;
}
String switch_crane(int p1[], int p2[], int s1, int s2) {
  String s = "";
  for (int i = 0; i < n; i++) {
    s += write_pin(p1[i], s1);
    s += write_pin(p2[i], s2);
  }
  return s;
}

void loop() {
  pause_crane();

  switchUp();

  pause_crane();

  switchDown();

}
