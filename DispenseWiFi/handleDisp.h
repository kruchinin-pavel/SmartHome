#include <stdexcept>
#if not defined(HANDLDISP_H)
#define HANDLDISP_H 1
#include <iostream>
#include "credentials.h"
#include "func.h"

ExtRegulatedDispenser disp{ 1, A0, "A0", D1, LED_BUILTIN};


/** Wifi config page handler */
void handleDispencer() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  String Page;

  Page += F(
            "<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<title>CaptivePortal</title></head><body>"
            "<h1>HELLO FROM DISPENSER!123!</h1>")
          + String(F("<p>Current level of input: ")) + analogRead(A0) + F("</p>")
          + String(F("<p>Current level of hummidity: ")) + disp.getReo() + F("</p>")
          ;
  Page += F("<p>You may want to <a href='/wifi'>config the wifi connection</a>.</p>");
  Page += String(F("<br /><form method='POST' action='dispSave'><h4>Setup humidity level(0-1024):</h4>"
                   "<input type='text' placeholder='humidity' name='humid' value='")) + disp.getReo() + String("'/>"
                       "<br /><input type='submit' value='Setup'/></form>"
                       "</body></html>");
  server.send(200, "text/html", Page);
}


void handleDispencerSetupLevel() {
  int humidity_limit = server.arg("humid").toInt();
  disp.setReo(humidity_limit);
  humidityLevel = humidity_limit;
  saveCredentials();
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}

void initDispenser() {
  disp.setReo(humidityLevel);
  server.on("/disp", handleDispencer);
  server.on("/dispSave", handleDispencerSetupLevel);
}
#endif
