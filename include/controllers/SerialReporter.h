#pragma once
#include "Arduino.h"

class SerialReporter
{
public:
    void begin()
    {
        Serial.println("Traffic controller starting...");
    }

    void reportInfo(const char *message)
    {
        Serial.print("Info: ");
        Serial.println(message);
    }

    void reportState(const char *stateName)
    {
        Serial.print("State: ");
        Serial.println(stateName);
    }

    void reportButtonPressed()
    {
        Serial.println("Pedestrian button: pressed");
    }
};
