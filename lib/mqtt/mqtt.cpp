#include "mqtt.h"
#include <genQrCode.h>

const char* ssid = "ThanhHoa";
const char* password = "1234567890";
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

unsigned long mqtt_lastUpdate = 0;
unsigned long mqtt_currentMillis = millis(); 

void connect_to_broker() {
    if (!client.connected()) {
        mqtt_currentMillis = millis();
        if(mqtt_currentMillis - mqtt_lastUpdate >= 2000){
            mqtt_lastUpdate = mqtt_currentMillis;
            Serial.print("Attempting MQTT connection...");
            String clientId = "ESP32";
            clientId += String(random(0xffff), HEX);
            if (client.connect(clientId.c_str())) {
                Serial.println("connected");
                updateNoticeLable_mqtt("Mqtt: Connect");
                client.subscribe(MQTT_TEXTQR_TOPIC);
                client.subscribe(MQTT_STATUS_TOPIC);
                client.subscribe(MQTT_REMOTE_TOPIC);
            } else {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 2 seconds");
                updateNoticeLable_mqtt("Mqtt: Disconnected");
            }
        }
    }else{
        mqtt_currentMillis = millis();
        mqtt_lastUpdate = mqtt_currentMillis;
    }
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("topic: ");
    Serial.println(topic);
    Serial.print("message: ");
    Serial.write(payload, length);
    Serial.println();

    char message[length + 1];           
    memcpy(message, payload, length);   
    message[length] = '\0';             
    Serial.print("message thanhpv2499: ");
    Serial.println(message);    

    if(accessQr){
        if(String(topic) == MQTT_TEXTQR_TOPIC)
        {
            update_qr_code_MQTT(message);
        }

        if(String(topic) == MQTT_REMOTE_TOPIC)
        {
            updateNoticeLable(message, true);
            publishMessage(MQTT_STATUS_TOPIC, message);
        }
    }        
}

bool setup_mqtt(){
     
    if(WiFi.status() != WL_CONNECTED) {
        if (isConnectWifi)
        {
            updateNoticeLable_mqtt("Wifi: Disconnected");
        }
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

void publishMessage(const char* topic, const char* message) {
    if (client.connected()) {
        bool success = client.publish(topic, message);
        if (success) {
            Serial.print("Message published to topic ");
            Serial.print(topic);
            Serial.println(": ");
            Serial.println(message);
        } else {
            Serial.println("Failed to publish message.");
        }
    } else {
        Serial.println("Client not connected. Cannot publish message.");
    }
}