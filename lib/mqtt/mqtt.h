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
#define MQTT_TEXTQR_TOPIC "MQTT_2499/TEXTQR"
#define MQTT_STATUS_TOPIC "MQTT_2499/STATUS"
#define MQTT_REMOTE_TOPIC "MQTT_2499/REMOTE"

void setup_wifi_mqtt();
void loop_mqtt();
void publishMessage(const char* topic, const char* message);

#endif
