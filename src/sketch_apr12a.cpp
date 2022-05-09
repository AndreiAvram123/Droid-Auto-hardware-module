
#include "Secrets.h"
#include "../lib/WiFiNINA/src/WiFiSSLClient.h"
#include "../lib/pubsubclient-2.8/src/PubSubClient.h"
#include "../lib/WiFiNINA/src/utility/wl_definitions.h"
#include "../lib/ArduinoJson-v6.19.4/ArduinoJson.h"
#include "../lib/WiFiNINA/src/WiFi.h"
#include <CarController.h>
#include <MqttClient.h>
#include "topics/UnlockTopic.h"
#include "topics/LockTopic.h"
#include "../lib/Arduino_MKRGPS-1.0.0/src/GPS.h"

using namespace std;
using namespace Car;

char ssid[] = NETWORK_SSID;     // your network SSID (name)
char pass[] = NETWORK_PASSWORD; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;    // the Wifi radio's status

int frontCarPIRPin = 6;
int leftDoorPIRPin = 1;
int pirStat = 0;                   // PIR status


unsigned long turnOnLightTime = 0;
unsigned long turnOffLightTime = 0;
unsigned long blinkInterval = 1000;

unsigned long connectionAttemptTime = 0;
unsigned long connectionAttemptInterval = 10000;
bool isUserInside = false;


void connectToWifi();
CarController carController;
MqttClient mqttClient;

void IndicateConnectingToWIFI();
void onMessageReceived(char* topic, byte* payload, unsigned int length);
void configureFrontCarPIR();
void configureLeftDoorPIR();
void readFrontCarPIR();
void readLeftDoorPIR();

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    configureFrontCarPIR();
    configureLeftDoorPIR();
    // Initialize serial and wait for port to open:


    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
       connectToWifi();
       IndicateConnectingToWIFI();
    }
     //initialize gps
    if (!GPS.begin()) {
        Serial.println("Failed to initialize GPS!");
        while (1);
    }

    mqttClient.initialize();
    mqttClient.setCallback(onMessageReceived);
    carController.configurePins();
    carController.lockCar();
}


void connectToWifi(){

    if(connectionAttemptTime < millis()){
        connectionAttemptTime = millis() + connectionAttemptInterval;
        Serial.print("Attempting to connect to network: ");
        IndicateConnectingToWIFI();
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
    }
}

void loop()
{
    mqttClient.loop();
    if (GPS.available()) {
        // read GPS values
        float latitude   = GPS.latitude();
        float longitude  = GPS.longitude();
        Location location = {
                .latitude = latitude,
                .longitude = longitude};

       mqttClient.updateLocation(location);
    }
    readFrontCarPIR();
    readLeftDoorPIR();
    mqttClient.updateIsUserInside(isUserInside)

    if (status == WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

void configureLeftDoorPIR(){
    pinMode(leftDoorPIRPin, INPUT);
}
void readLeftDoorPIR(){
    pirStat = digitalRead(leftDoorPIRPin);
    if(pirStat == HIGH){
        isUserInside = true;
        string  toPrint ="Left door sensor detected movement" + to_string(millis());
        Serial.println(toPrint.c_str());
    }
}

 void configureFrontCarPIR(){
     pinMode(frontCarPIRPin, INPUT);
 }

void readFrontCarPIR(){
    pirStat = digitalRead(frontCarPIRPin);
    if(pirStat == HIGH){
        isUserInside = true;
        string  toPrint ="Front PRI sensor detected movement" + to_string(millis());
        Serial.println(toPrint.c_str());
    }
}

void IndicateConnectingToWIFI()
{

    if (turnOnLightTime < millis())
    {

        turnOnLightTime = millis() + blinkInterval * 2;
        turnOffLightTime = millis() + blinkInterval;

        digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (turnOffLightTime < millis())
    {

        digitalWrite(LED_BUILTIN, LOW);
    }
}



void onMessageReceived(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    if(strcmp(topic,LockTopic::getRoute().c_str()) == 0){
         Serial.println("Locking car...");
         carController.lockCar();
    }
    if(strcmp(topic,UnlockTopic::getRoute().c_str()) == 0){
        Serial.println("Unlocking car...");
        carController.unlockCar();
    }
}
