//
// Created by Andrei Avram on 13.04.2022.
//

#include "UnlockTopic.h"
#include "Secrets.h"

string UnlockTopic::getRoute() {
    string unlockTopic = "/cars/" + string (CAR_ID) + "/unlock";
    return unlockTopic;
}
