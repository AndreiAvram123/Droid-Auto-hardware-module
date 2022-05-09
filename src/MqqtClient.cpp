//
// Created by Andrei Avram on 13.04.2022.
//


#include <variant.h>
#include <USB/USBAPI.h>
#include "iostream"
#include "MqttClient.h"
#include "../lib/WiFiNINA/src/WiFiSSLClient.h"
#include "../lib/pubsubclient-2.8/src/PubSubClient.h"
#include "Secrets.h"
#include "../lib/ArduinoJson-v6.19.4/ArduinoJson.h"
#include "topics/UnlockTopic.h"
#include "topics/LockTopic.h"
using namespace std;


const char *mqtt_server = "3d6ce0046c514d07b3f9e653a9c67302.s1.eu.hivemq.cloud";
string carID = "10";
WiFiSSLClient wifiClient;

unsigned long nextLocationUpdate = 0;
unsigned long locationUpdateInterval = 5000;

PubSubClient pubSubClient(wifiClient);

void MqttClient::loop() {
    pubSubClient.loop();
}


void subscribeToTopics(){
    pubSubClient.subscribe(UnlockTopic::getRoute().c_str());
    pubSubClient.subscribe(LockTopic::getRoute().c_str());
}


void MqttClient::initialize() {
    pubSubClient.setServer(mqtt_server, 8883);
    while (!pubSubClient.connected())
    {
        Serial.println("connecting to mqtt");
        if (pubSubClient.connect("arduino-client", MQTT_USERNAME, MQTT_PASSWORD))
        {
            subscribeToTopics();
            Serial.println("success");
        }
        else
        {
            Serial.println("failure");
            Serial.println("Try again in 5 sec");

            delay(5000);
        }
    }

}
void MqttClient::updateLocation(Location newLocation) {

    if (millis() > 3600000 && nextLocationUpdate > 0)
    {
        nextLocationUpdate = 0;
    }
    if (nextLocationUpdate < millis())
    {

        nextLocationUpdate = millis() + locationUpdateInterval;
        string topic = "/cars/" + carID + "/location";
        DynamicJsonDocument location(1024);
        location["latitude"] = newLocation.latitude;
        location["longitude"] = newLocation.longitude;
        char json_string[1024];
        serializeJson(location, json_string);
        Serial.println("Updating location with");
        Serial.print(newLocation.latitude);
        Serial.print(newLocation.longitude);
        pubSubClient.publish(topic.c_str(), json_string);
    }
}

void MqttClient::setCallback(void (*callback)(char *, uint8_t *, unsigned int)) {
    pubSubClient.setCallback(callback);
}

void MqttClient::updateIsUserInside(bool isUserInside) {
    //todo
    //update
}

