#include <MKRGSM.h>
#include <WiFi.h> //You need to have the arduino library with Wifi.h
#include <PubSubClient.h> //You need to have pubsubclient library with Pubsubclient.h

//This script has been made for ESP32.
//Same script basically works with other arduinos, but you need to have necessary network connectivity determined. 

//const char* ssid = "Your wifi SSID";
//const char* password =  "Your wifi password";
const char* mqttServer = "3.125.148.52"; //Amazon server IP, do not change
const int mqttPort = 1883; //MQTT port on Amazon server, do not change
const char* mqttUser = "device"; //pre-determined user for the amazon server MQTT client, do not change
const char* mqttPassword = "fnp26K3gcAqy"; //Pre-determined password for amazon server mqtt client, do not change

/*#define SECRET_PIN ""
#define SECRET_GPRS_APN internet
#define SECRET_GPRS_LOGIN "" 
#define SECRET_GPRS_PASS "" */
// PIN Number
const char PINNUMBER[]     ="";
// APN data
const char GPRS_APN[]      = "internet";
const char GPRS_LOGIN[]    =""; 
const char GPRS_PASSWORD[] ="";

GSMClient gsmclient;
GPRS gprs;
GSM gsmAccess;

//WiFiClient espClient; 
PubSubClient client(gsmclient);
int number = 0;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("Re-connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {

  Serial.begin(115200);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }*/

 // WiFi.begin(ssid, password);

 // while (WiFi.status() != WL_CONNECTED) {
   // delay(500);
   // Serial.println("Connecting to WiFi..");
  //}
  Serial.println("Connecting to GSM");
  boolean connected = false;

  delay(3000);

  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connected");
  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client1", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // client.publish("jnmtrp", "1,45"); // 0,X for delete entry; 1,X for add entry; X = fishlenth in cm.
  // e.g. client.publish("vbnhzm", "1,45"); would add a fish with length of 45 cm to database.

  Serial.println("Message to be sent");
  Serial.println("ykbtff, 1,33"); // put here the Trap ID in place of the "pwvlkw"
  client.publish("ykbtff", "1,33"); // put here the Trap ID in place of the "pwvlkw"

  delay(5000);

}
