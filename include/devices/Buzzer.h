#pragma once
#include "Arduino.h"

class Buzzer
{  
private:
    uint8_t _pin;
    uint16_t _frequency;
    uint8_t _channel;
    uint8_t _resolution;
    bool _isOn;
public:
    Buzzer(uint8_t pin, uint16_t frequency = 2000, uint8_t channel = 0, uint8_t resolution = 8)
        : _pin(pin), _frequency(frequency), _channel(channel), _resolution(resolution), _isOn(false) {}
    void begin()
    {
        pinMode(_pin, OUTPUT);
        #if defined(ARDUINO_ARCH_ESP32)
        ledcSetup(_channel, _frequency, _resolution);
        ledcAttachPin(_pin, _channel);
        #endif
        off();
    }
    void on()
    {
        #if defined(ARDUINO_ARCH_ESP32)
        ledcWriteTone(_channel, _frequency);
        #else
        tone(_pin, _frequency);
        #endif
        _isOn = true;
    }
    void off()
    {
        #if defined(ARDUINO_ARCH_ESP32)
        ledcWriteTone(_channel, 0);
        #else
        noTone(_pin);
        #endif
        _isOn = false;
    }
    void toggle()
    {
        if (_isOn)
        {
            off();
        }
        else
        {
            on();
        }
    }
    void setFrequency(uint16_t frequency)
    {
        _frequency = frequency;
        if (_isOn)
        {
            on();
        }
    }
};