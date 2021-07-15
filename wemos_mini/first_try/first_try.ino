#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


// Replace with your network credentials
const char* ssid = "<YOUR WIFI SSID>";
const char* password = "<YOUR WIFI PASSWORD>";
ESP8266WebServer server(80);

String index = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";

void setup(void) {

  //the HTML of the web page
  
//make the LED pin output and initially turned off
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    
    delay(1000);
    Serial.begin(115200);
    WiFi.softAP(ssid, password); //begin WiFi access point
    Serial.println("");
    
    server.on("/", []() {
        server.send(200, "text/html", index);
      
  });
    server.on("/LEDOn", []() {
        server.send(200, "text/html", index);
        digitalWrite(13, HIGH);
        delay(1000);
      
  });
    server.on("/LEDOff", []() {
        server.send(200, "text/html", index);
        digitalWrite(13, LOW);
        delay(1000);
      
  });
    server.begin();
    Serial.println("Web server started!");
}
void loop(void) {
    server.handleClient();
}
