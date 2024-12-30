#include "mqtt.h"

String ledStatus1 = "ON";
String ledStatus2 = "ON";
const char* ssid = "FPT_CT";
const char* password = "123456789";
unsigned long previousMillis = 0; 
const long interval = 1000;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
String topicLates = " ";
bool isConnectWifi = false;

void setup_wifi_mqtt() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password); 
}

void connect_to_broker() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(MQTT_LED1_TOPIC);
            client.subscribe(MQTT_LED2_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            delay(2000);
        }
    }
}

void callback(char* topic, byte *payload, unsigned int length) {
    char status[20];
    Serial.println("-------new message from broker-----");
    Serial.print("topic: ");
    Serial.println(topic);
    Serial.print("message: ");
    Serial.write(payload, length);
    Serial.println();
    for(int i = 0; i<length; i++)
    {
        status[i] = payload[i];
    }
    Serial.println(status);
    topicLates = String(topic);
    if(String(topic) == MQTT_LED1_TOPIC)
    {
        if(String(status) == "OFF")
        {
            ledStatus1 = "OFF";
            digitalWrite(LED1, LOW);
            Serial.println("LED1 OFF");
        }
        else if(String(status) == "ON")
        {
            ledStatus1 = "ON";
            digitalWrite(LED1, HIGH);
            Serial.println("LED1 ON");
        }
    }

    if(String(topic) == MQTT_LED2_TOPIC)
    {
        if(String(status) == "OFF")
        {
            ledStatus2 = "OFF";
            digitalWrite(LED2, LOW);
            Serial.println("LED2 OFF");
        }
        else if(String(status) == "ON")
        {
            ledStatus2 = "ON";
            digitalWrite(LED2, HIGH);
            Serial.println("LED2 ON");
        }
    }
}

bool setup_mqtt(){
     
    if(WiFi.status() != WL_CONNECTED) {
        isConnectWifi = false;
    }else if(isConnectWifi == false) {
        isConnectWifi = true;
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        client.setServer(MQTT_SERVER, MQTT_PORT );
        client.setCallback(callback);
        connect_to_broker();
        Serial.println("Start transfer");
    }

    return isConnectWifi;
}

void loop_mqtt()
{
    if(setup_mqtt())
    {
        client.loop();
        if (!client.connected())
        {
            connect_to_broker();
        }else{
        }
    }
}