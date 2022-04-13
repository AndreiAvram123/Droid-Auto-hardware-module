//
// Created by Andrei Avram on 13.04.2022.
//

#ifndef CAR_RENTAL_ARDUINO_MQTTCLIENT_H
#define CAR_RENTAL_ARDUINO_MQTTCLIENT_H

#endif //CAR_RENTAL_ARDUINO_MQTTCLIENT_H

#include "iostream"
#include "../lib/pubsubclient-2.8/tests/src/lib/Arduino.h"
#include "../lib/pubsubclient-2.8/src/PubSubClient.h"
#include "Location.h"


class MqttClient{
public:
       void loop();
public :
       void initialize();
public:
    void updateLocation(Location newLocation);


public :
      void setCallback(MQTT_CALLBACK_SIGNATURE);
};

