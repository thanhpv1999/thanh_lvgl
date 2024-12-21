#ifndef MQTT_h
#define MQTT_h

#include <WiFi.h>
#include <PubSubClient.h>

#define LED1 17
#define LED2 15
#define MQTT_SERVER "mqtt-dashboard.com"
#define MQTT_PORT 1883
#define MQTT_USER "mabattu123"
#define MQTT_PASSWORD "12345678"
#define MQTT_LED1_TOPIC "MQTT_ESP32/LED1"
#define MQTT_LED2_TOPIC "MQTT_ESP32/LED2"

void setup_mqtt();
void loop_mqtt();

#endif
