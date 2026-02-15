#pragma once
#include "Arduino.h"
#include "devices/Button.h"
#include "devices/Buzzer.h"
#include "lights/TrafficLight.h"
#include "lights/PedestrianLight.h"
#include "controllers/SerialReporter.h"

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
    SerialReporter *_reporter;
    TrafficState _state;
    unsigned long _lastStateChangeTime;

    TrafficState _trafficState;
    unsigned long _stateStart;
    unsigned long _lastBuzzerToggle;
    bool _buzzerPatternOn;

    const unsigned long VEHICLE_GREEN_DURATION = 15000;
    const unsigned long VEHICLE_YELLOW_DURATION = 3000;
    const unsigned long VEHICLE_RED_DURATION = 15000;
    const unsigned long PEDESTRIAN_CROSS_DURATION = 20000;
    const unsigned long BUZZER_ON_DURATION = 200;
    const unsigned long BUZZER_OFF_DURATION = 300;

public:
    TrafficController(TrafficLight trafficLight, PedestrianLight pedestrianLight, Button button, Buzzer buzzer, SerialReporter *reporter = nullptr)
        : _trafficLight(trafficLight), _pedestrianLight(pedestrianLight), _button(button), _buzzer(buzzer), _reporter(reporter), _state(TrafficState::VEHICLE_GREEN), _lastStateChangeTime(0) {}

    void begin()
    {
        _trafficLight.begin();
        _pedestrianLight.begin();
        _button.begin();
        _buzzer.begin();
        _trafficState = TrafficState::VEHICLE_GREEN;
        _stateStart = millis();
        _lastBuzzerToggle = _stateStart;
        _buzzerPatternOn = false;
        applyState(_trafficState);
        if (_reporter)
        {
            _reporter->begin();
            _reporter->reportInfo("Controller initialized");
            _reporter->reportState(trafficStateName(_trafficState));
        }
    }

    void update()
    {
        unsigned long currentTime = millis();
        switch (_trafficState)
        {
        case TrafficState::VEHICLE_GREEN:
            if (_button.isPressed())
            {
                if (_reporter)
                {
                    _reporter->reportButtonPressed();
                }
                _trafficState = TrafficState::VEHICLE_YELLOW;
                _stateStart = currentTime;
                applyState(_trafficState);
                reportStateIfNeeded(_trafficState);
            }
            else if (currentTime - _stateStart >= VEHICLE_GREEN_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_YELLOW;
                _stateStart = currentTime;
                applyState(_trafficState);
                reportStateIfNeeded(_trafficState);
            }
            break;

        case TrafficState::VEHICLE_YELLOW:
            if (currentTime - _stateStart >= VEHICLE_YELLOW_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_RED;
                _stateStart = currentTime;
                applyState(_trafficState);
                reportStateIfNeeded(_trafficState);
                if (_reporter)
                {
                    _reporter->reportInfo("Pedestrian crossing: buzzer pattern start");
                }
            }
            break;

        case TrafficState::VEHICLE_RED:
            updateBuzzerPattern(currentTime, true);
            if (currentTime - _stateStart >= PEDESTRIAN_CROSS_DURATION)
            {
                _trafficState = TrafficState::VEHICLE_GREEN;
                _stateStart = currentTime;
                applyState(_trafficState);
                reportStateIfNeeded(_trafficState);
                if (_reporter)
                {
                    _reporter->reportInfo("Pedestrian crossing: buzzer pattern stop");
                }
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

    void applyState(TrafficState newState)
    {
        switch (newState)
        {
        case TrafficState::VEHICLE_GREEN:
            _trafficLight.greenOn();
            _pedestrianLight.stopCrossing();
            _buzzer.off();
            _buzzerPatternOn = false;
            break;
        case TrafficState::VEHICLE_YELLOW:
            _trafficLight.yellowOn();
            _pedestrianLight.stopCrossing();
            _buzzer.off();
            _buzzerPatternOn = false;
            break;
        case TrafficState::VEHICLE_RED:
            _trafficLight.redOn();
            _pedestrianLight.allowCross();
            _buzzer.off();
            _buzzerPatternOn = false;
            _lastBuzzerToggle = millis();
            break;
        case TrafficState::PEDESTRIAN_GREEN:
            _trafficLight.redOn();
            _pedestrianLight.allowCross();
            _buzzer.off();
            _buzzerPatternOn = false;
            _lastBuzzerToggle = millis();
            break;
        case TrafficState::PEDESTRIAN_RED:
            _trafficLight.greenOn();
            _pedestrianLight.stopCrossing();
            _buzzer.off();
            _buzzerPatternOn = false;
            break;
        }
    }

    void updateBuzzerPattern(unsigned long currentTime, bool enabled)
    {
        if (!enabled)
        {
            if (_buzzerPatternOn)
            {
                _buzzer.off();
                _buzzerPatternOn = false;
            }
            return;
        }

        unsigned long interval = _buzzerPatternOn ? BUZZER_ON_DURATION : BUZZER_OFF_DURATION;
        if (currentTime - _lastBuzzerToggle >= interval)
        {
            if (_buzzerPatternOn)
            {
                _buzzer.off();
                _buzzerPatternOn = false;
            }
            else
            {
                _buzzer.on();
                _buzzerPatternOn = true;
            }
            _lastBuzzerToggle = currentTime;
        }
    }

    void reportStateIfNeeded(TrafficState newState)
    {
        if (_reporter)
        {
            _reporter->reportState(trafficStateName(newState));
        }
    }

    const char *trafficStateName(TrafficState state)
    {
        switch (state)
        {
        case TrafficState::VEHICLE_GREEN:
            return "VEHICLE_GREEN";
        case TrafficState::VEHICLE_YELLOW:
            return "VEHICLE_YELLOW";
        case TrafficState::VEHICLE_RED:
            return "VEHICLE_RED";
        case TrafficState::PEDESTRIAN_GREEN:
            return "PEDESTRIAN_GREEN";
        case TrafficState::PEDESTRIAN_RED:
            return "PEDESTRIAN_RED";
        default:
            return "UNKNOWN";
        }
    }
};