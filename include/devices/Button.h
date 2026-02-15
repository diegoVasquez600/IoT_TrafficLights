#pragma once
#include "Arduino.h"

class Button
{
private:
    uint8_t _pin;
    bool _lastReading;
    bool _stableState;
    unsigned long _lastDebounceTime;
    const unsigned long _debounceDelay;

public:
    Button(uint8_t pin, unsigned long debounceDelay = 50) : _pin(pin), _debounceDelay(debounceDelay) {}

    void begin()
    {
        pinMode(_pin, INPUT_PULLUP);
        _lastReading = digitalRead(_pin);
        _stableState = _lastReading;
        _lastDebounceTime = 0;
    }

    bool isPressed()
    {
        bool reading = digitalRead(_pin);
        if (reading != _lastReading)
        {
            _lastDebounceTime = millis();
        }
        if ((millis() - _lastDebounceTime) > _debounceDelay)
        {
            if (reading != _stableState)
            {
                _stableState = reading;
                if (_stableState == LOW)
                {
                    _lastReading = reading;
                    return true;
                }
            }
        }
        _lastReading = reading;
        return false;
    }
};