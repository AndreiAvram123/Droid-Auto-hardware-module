//
// Created by Andrei Avram on 13.04.2022.
//
#include "CarController.h"
using namespace Car;

void CarController::lockCar() {
    WiFiDrv::analogWrite(25, 150);
    WiFiDrv::analogWrite(26, 0);
    WiFiDrv::analogWrite(27, 0);
}

void CarController::configurePins() {
    // green pin
    WiFiDrv::pinMode(25, OUTPUT);
//red pin
    WiFiDrv::pinMode(26,OUTPUT);
//blue pin
    WiFiDrv::pinMode(27,OUTPUT);
}


void CarController::unlockCar() {
    WiFiDrv::analogWrite(25, 0);
    WiFiDrv::analogWrite(26, 150);
    WiFiDrv::analogWrite(27, 0);
}
