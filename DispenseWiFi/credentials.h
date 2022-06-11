#if not defined(CREDENTIALS_H)
#define CREDENTIALS_H

#include "context.h"

/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
  char ok[2 + 1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), humidityLevelMin);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(humidityLevelMin), humidityLevelMax);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password) > 0 ? "********" : "<no password>");
  Serial.print("Hum level: ");
  Serial.println( humidityLevelMin);
  Serial.print(" - ");
  Serial.println( humidityLevelMax);
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
  char ok[2 + 1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), humidityLevelMin);
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(humidityLevelMin), humidityLevelMax);
  EEPROM.commit();
  EEPROM.end();
}

#endif
