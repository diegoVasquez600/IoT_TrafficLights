#pragma once
#include "Arduino.h"

class Led
{
private:
    uint8_t _pin;

public:
    Led(uint8_t pin) : _pin(pin) {}
    void begin()
    {
        pinMode(_pin, OUTPUT);
        off();
    }
    void on()
    {
        digitalWrite(_pin, HIGH);
    }
    void off()
    {
        digitalWrite(_pin, LOW);
    }
    void toggle()
    {
        digitalWrite(_pin, !digitalRead(_pin));
    }
};
