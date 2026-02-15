#pragma once
#include "devices/Led.h"

class PedestrianLight
{
private:
    Led _ledGreen;
    Led _ledRed;

public:
    PedestrianLight(uint8_t ledGreenPin, uint8_t ledRedPin) : _ledGreen(ledGreenPin), _ledRed(ledRedPin) {}
    void begin()
    {
        _ledGreen.begin();
        _ledRed.begin();
    }
    void allowCross()
    {
        _ledGreen.on();
        _ledRed.off();
    }
    void stopCrossing()
    {
        _ledGreen.off();
        _ledRed.on();
    }
    void off()
    {
        _ledGreen.off();
        _ledRed.off();
    }
};