#if not defined(MQTT_H)
#define MQTT_H 1

#include <PubSubClient.h>
#include <ArduinoJson.h>

void callback(char* topic, byte* payload, unsigned int length);

PubSubClient client(mqtt_server, 1883, callback, wifiClient);

// Читаем MQTT сообщения
void callback(char* topic, byte* payload, unsigned int length) {
}


// Собираем данные, складываем воедино и отправляем
void report(double humidity, double minLevel, double maxLevel) {
  StaticJsonDocument<500> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["humidity"] = humidity;
  root["limitMin"] = minLevel;
  root["limitMax"] = maxLevel;
  char payload[256];
  serializeJson(jsonBuffer, payload);
  char* s = payload;
  if (client.connect("ESP8266 dispenser")) {
    if (client.publish("ESP/Dispenser1", s) == true) {
      Serial.println("Sent!");
      delay(2000);
//      ESP.deepSleep(5e6); // 20e6 это и есть deep-sleep. Засыпаем на 20 секунд!
    }
  }
}

#endif
