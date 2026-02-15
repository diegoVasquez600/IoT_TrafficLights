#pragma once
#include "Arduino.h"
#include "devices/Led.h"

class TrafficLight
{
private:
    Led _red;
    Led _yellow;
    Led _green;

public:
    TrafficLight(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin) : _red(redPin), _yellow(yellowPin), _green(greenPin) {}
    void begin()
    {
        _red.begin();
        _yellow.begin();
        _green.begin();
    }
    void offAll()
    {
        _red.off();
        _yellow.off();
        _green.off();
    }
    void greenOn()
    {
        offAll();
        _green.on();
    }
    void yellowOn()
    {
        offAll();
        _yellow.on();
    }
    void redOn()
    {
        offAll();
        _red.on();
    }
    void toggleGreen()
    {
        _green.toggle();
    }
    void toggleYellow()
    {
        _yellow.toggle();
    }
    void toggleRed()
    {
        _red.toggle();
    }
};