#if not defined(CREDENTIALS_H)
#define CREDENTIALS_H

#include "context.h"

template<typename T>
void eeread(T& value, int size_of, int& ioffset) {
  EEPROM.get(ioffset, value);
  ioffset += size_of;
}


template<typename T>
void eeput(T& value, int size_of, int& ioffset) {
  EEPROM.put(ioffset, value);
  ioffset += size_of;
}

/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  auto ioffset = 0;
  //  EEPROM.get(0, ssid);
  eeread(ssid, sizeof(ssid), ioffset);
  eeread(password, sizeof(password), ioffset);
  char ok[2 + 1];
  eeread(ok, sizeof(ok), ioffset);
  eeread(humidityLevelMin, sizeof(humidityLevelMin), ioffset);
  eeread(humidityLevelMax, sizeof(humidityLevelMax), ioffset);
  char ip_c[100];
  eeread(ip_c, sizeof(ip_c), ioffset);
  mqtt_server.fromString(ip_c);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password) > 0 ? "********" : "<no password>");
  Serial.printf("Hum level: %d - %d\n", humidityLevelMin, humidityLevelMax);
  Serial.printf("MQTT Sever: %s\n", mqtt_server.toString().c_str());
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  auto ioffset = 0;
  eeput(ssid, sizeof(ssid), ioffset);
  eeput(password, sizeof(password), ioffset);
  char ok[2 + 1] = "OK";
  eeput(ok, sizeof(ok), ioffset);
  eeput(humidityLevelMin, sizeof(humidityLevelMin), ioffset);
  eeput(humidityLevelMax, sizeof(humidityLevelMax), ioffset);

  auto s = mqtt_server.toString().c_str();
  eeput(s, sizeof(s), ioffset);
  EEPROM.commit();
  EEPROM.end();
  loadCredentials();
}

#endif
