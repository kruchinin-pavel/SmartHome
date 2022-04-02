#if not defined(CONTEXT_H)
#define CONTEXT_H 1
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

/*
   This example serves a "hello world" on a WLAN and a SoftAP at the same time.
   The SoftAP allow you to configure WLAN parameters at run time. They are not setup in the sketch but saved on EEPROM.

   Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config. If it does not then navigate to http://192.168.4.1/wifi and config it there.
   Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.

   Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.

   This is a captive portal because through the softAP it will redirect any http request to http://192.168.4.1/
*/

/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID "Dispenser_ap"
#define APPSK  "12345678"
#endif

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[65] = "";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;

unsigned int humidityLevel = 100000;

#endif
