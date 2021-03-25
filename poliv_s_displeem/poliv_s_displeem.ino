#include <splash.h>
#include <Adafruit_SSD1306.h>

#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>

#define DHTPIN 2     // контакт, к которому подключаемся 
#define DHTTYPE DHT11   // для датчика DHT11
#define OLED_RESET 4
#include "func.h"
Adafruit_SSD1306 display(OLED_RESET);


void setup()
{
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // инициализируем объект «display» с I2C-адресом «0x3C», для 128x32
  Serial.begin(9600);
}

void show_msg(int lineNo, String msg, float param){
   display.setCursor(0,10*lineNo);
   display.print(msg);  
   display.print(param);
 
}

void displayTempHumid(){
  delay(100);
  // считывание данных о температуре и влажности 
  // занимает около 250 мс; кроме того, это считывание
  // может запаздывать примерно на 2 секунды 
  // (это очень медленный датчик):
  float h = analogThrustedRead(A1);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  show_msg(0, "Humidity: ", h);
}
void loop()
{
  displayTempHumid();
  display.display();
}
