#pragma once
#include "Arduino.h"
#include "devices/Button.h"
#include "devices/Buzzer.h"
#include "lights/TrafficLight.h"
#include "lights/PedestrianLight.h"

enum class TrafficState
{
    VEHICLE_GREEN,
    VEHICLE_YELLOW,
    VEHICLE_RED,
    PEDESTRIAN_GREEN,
    PEDESTRIAN_RED
};

class TrafficController
{
private:
    TrafficLight _trafficLight;
    PedestrianLight _pedestrianLight;
    Button _button;
    Buzzer _buzzer;
    TrafficState _state;
    unsigned long _lastStateChangeTime;

    TrafficState _trafficState;
    unsigned long _stateStart;

    const unsigned long VEHICLE_GREEN_DURATION = 15000;
    const unsigned long VEHICLE_YELLOW_DURATION = 3000;
    const unsigned long VEHICLE_RED_DURATION = 15000;
    const unsigned long PEDESTRIAN_CROSS_DURATION = 20000;

public:
    TrafficController(TrafficLight trafficLight, PedestrianLight pedestrianLight, Button button, Buzzer buzzer)
        : _trafficLight(trafficLight), _pedestrianLight(pedestrianLight), _button(button), _buzzer(buzzer), _state(TrafficState::VEHICLE_GREEN), _lastStateChangeTime(0) {}

    void begin()
    {
        _trafficState = TrafficState::VEHICLE_GREEN;
        _stateStart = millis();
        _trafficLight.begin();
        _trafficLight.greenOn();
        _pedestrianLight.begin();
        _pedestrianLight.stopCrossing();
        _button.begin();
        _buzzer.begin();
    }

    void update()
    {
        unsigned long currentTime = millis();
        switch (_trafficState)
        {
        case TrafficState::VEHICLE_GREEN:
            if (_button.isPressed())
            {
                _trafficState = TrafficState::VEHICLE_YELLOW;
                _stateStart = currentTime;
                _trafficLight.yellowOn();
            }
            else if (currentTime - _stateStart >= VEHICLE_GREEN_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_YELLOW;
                _stateStart = currentTime;
                _trafficLight.yellowOn();
            }
            break;

        case TrafficState::VEHICLE_YELLOW:
            if (currentTime - _stateStart >= VEHICLE_YELLOW_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_RED;
                _stateStart = currentTime;
                _trafficLight.redOn();
                _pedestrianLight.allowCross();
                _buzzer.on();
            }
            break;

        case TrafficState::VEHICLE_RED:
            if (currentTime - _stateStart >= PEDESTRIAN_CROSS_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_GREEN;
                _stateStart = currentTime;
                _trafficLight.greenOn();
                _pedestrianLight.stopCrossing();
                _buzzer.off();
            }
            break;

        default:
            break;
        }
    }

private:
    bool elapsed(unsigned long duration)
    {
        return (millis() - _lastStateChangeTime) >= duration;
    }

    void changeState(TrafficState newState)
    {
        _state = newState;
        _lastStateChangeTime = millis();

        switch (newState)
        {
        case TrafficState::VEHICLE_GREEN:
            _trafficLight.greenOn();
            _pedestrianLight.stopCrossing();
            _buzzer.off();
            break;
        case TrafficState::VEHICLE_YELLOW:
            _trafficLight.yellowOn();
            _pedestrianLight.stopCrossing();
            _buzzer.off();
            break;
        case TrafficState::VEHICLE_RED:
            _trafficLight.redOn();
            _pedestrianLight.allowCross();
            _buzzer.on();
            break;
        case TrafficState::PEDESTRIAN_GREEN:
            _trafficLight.redOn();
            _pedestrianLight.allowCross();
            _buzzer.on();
            break;
        }
    }
};