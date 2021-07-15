int dataPin  = 9;   //Пин подключен к DS входу 74HC595
int latchPin = 10;  //Пин подключен к ST_CP входу 74HC595
int clockPin = 11;  //Пин подключен к SH_CP входу 74HC595

void setup() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  
}
 
void loop() {

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B00000000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  delay(1000);

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B11111111);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  delay(1000);
}
