#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "ssid";
const char* password =  "password";
const char* mqttServer = "serverIP";
const int mqttPort = 1883;
const char* mqttUser = "username";
const char* mqttPassword = "password";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("vbnhzm", "0,45"); // 0,X for delete entry; 1,X for add entry; X = fishlenth in cm.
  // e.g. client.publish("vbnhzm", "1,45"); would add a fish with length of 45 cm to database.

  
  
  
}
 
void loop() {
  client.loop();
}
