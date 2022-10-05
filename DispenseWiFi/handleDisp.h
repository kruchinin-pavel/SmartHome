#include <stdexcept>
#if not defined(HANDLDISP_H)
#define HANDLDISP_H 1
#include <iostream>
#include "credentials.h"
#include "func.h"

PumpDriver link{D1};
ExtRegulatedDispenser disp( 1, A0, "A0", LED_BUILTIN, link);


/** Wifi config page handler */
void handleDispencer() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  String Page;

  Page += String(
            "<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<title>CaptivePortal</title></head><body>"
            "<h1>Dispenser config:</h1>")
          + "<p>Current dryness [0;1024]: " + disp.getCurrent() + "</p>"
          + "<p>Keep in limits: [" + disp.getReoMin() + " ; " + disp.getReoMax() + "]</p>"
          + "<p>Pump state: " + disp.getState() + "</p>"          ;
  Page += "<p>You may want to <a href='/wifi'>config the wifi connection</a>.</p>";
  Page += String("<br />")
          + "<form method='POST' action='dispSave'><h4>Setup range [0-1024]:</h4>"
          + "from <input type='text' placeholder='hmin' name='humidMin' value='" + disp.getReoMin() + "'/> "
          + "to <input type='text' placeholder='hmax' name='humidMax' value='" + disp.getReoMax() + "'/><br />"
          + "<input type='submit' value='Setup'/>" +
          +"</form>";

  Page += "<br /><form method='POST' action='doPump'><input type='submit' value='do dispence'/></form>";
  Page += "<form method='POST' action='stopPump'><input type='submit' value='stop dispence'/></form>";
  Page += "</body></html>";

  server.send(200, "text/html", Page);
}

void handleDispencerDoPump() {
  disp.doPump();
  server.sendHeader("Location", "disp", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}


void handleDispencerStopPump() {
  disp.stopPump();
  server.sendHeader("Location", "disp", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}

void handleDispencerSetupLevel() {
  int humidity_limit_min = server.arg("humidMin").toInt();
  int humidity_limit_max = server.arg("humidMax").toInt();
  disp.setReoMin(humidity_limit_min);
  disp.setReoMax(humidity_limit_max);
  humidityLevelMin = disp.getReoMin();
  humidityLevelMax = disp.getReoMax();
  saveCredentials();
  server.sendHeader("Location", "disp", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}

void initDispenser() {
  Serial.print("initDispenser");
  disp.setReoMin(humidityLevelMin);
  disp.setReoMax(humidityLevelMax);
  server.on("/disp", handleDispencer);
  server.on("/dispSave", handleDispencerSetupLevel);
  server.on("/doPump", handleDispencerDoPump);
  server.on("/stopPump", handleDispencerStopPump);
  Serial.println("...(done)");
}
#endif
