//
// Created by Andrei Avram on 14.04.2022.
//

#include "LockTopic.h"
#include "Secrets.h"


string LockTopic::getRoute() {
    string unlockTopic = "/cars/" + string (CAR_ID) + "/lock";
    return unlockTopic;
}