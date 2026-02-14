#pragma once
#include "Arduino.h"

class Button
{
private:
    uint8_t _pin;
    bool _lastState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay;

public:
    Button(uint8_t pin, unsigned long debounceDelay = 50) : _pin(pin), _debounceDelay(debounceDelay) {}

    void begin()
    {
        pinMode(_pin, INPUT_PULLUP);
        _lastState = digitalRead(_pin);
        _lastDebounceTime = 0;
    }

    bool isPressed()
    {
        bool reading = digitalRead(_pin);
        if (reading != _lastState)
        {
            _lastDebounceTime = millis();
        }
        if ((millis() - _lastDebounceTime) > _debounceDelay)
        {
            if (reading == LOW && _lastState == HIGH)
            {
                _lastState = reading;
                return true;
            }
        }
        _lastState = reading;
        return false;
    }
};