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

void setup_wifi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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

void setup_mqtt(){
    //Serial.setTimeout(500);
    setup_wifi();
    client.setServer(MQTT_SERVER, MQTT_PORT );
    client.setCallback(callback);
    connect_to_broker();
    Serial.println("Start transfer");
    // pinMode(LED1, OUTPUT);
    // pinMode(LED2, OUTPUT);
}

void loop_mqtt()
{
    client.loop();
    if (!client.connected())
    {
        connect_to_broker();
    }else{
        // unsigned long currentMillis = millis();  
        // if (currentMillis - previousMillis >= interval) {  
        //     previousMillis = currentMillis;
        //     Serial.println("Sending message every second");
        //     if(topicLates != " "){
        //         client.publish(topicLates.c_str(), "hi"); 
        //     }
        // }
    }
}