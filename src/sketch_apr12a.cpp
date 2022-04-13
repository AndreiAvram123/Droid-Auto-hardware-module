
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

using namespace std;
using namespace Car;

char ssid[] = NETWORK_SSID;     // your network SSID (name)
char pass[] = NETWORK_PASSWORD; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;    // the Wifi radio's status

unsigned long turnOnLightTime = 0;
unsigned long turnOffLightTime = 0;
unsigned long blinkInterval = 1000;

unsigned long connectionAttemptTime = 0;
unsigned long connectionAttemptInterval = 10000;
void connectToWifi();
CarController carController;
MqttClient mqttClient;


double fRand(double fMin, double fMax);
void IndicateConnectingToWIFI();
void onMessageReceived(char* topic, byte* payload, unsigned int length);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    // Initialize serial and wait for port to open:


    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
       connectToWifi();
       IndicateConnectingToWIFI();
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
    Location randomLocation = {
            .latitude = 53.4677543,
            .longitude = -2.2784265};

    mqttClient.updateLocation(randomLocation);


    if (status == WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
     delay(300);
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

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}