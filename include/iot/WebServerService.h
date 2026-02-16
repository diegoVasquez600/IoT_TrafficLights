#pragma once
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "controllers/TrafficController.h"

class WebServerService
{

private:
    AsyncWebServer _server;
    TrafficController *_controller;
public:
    WebServerService(TrafficController *controller, uint16_t port = 80) : _server(port), _controller(controller) {}
    void begin()
    {
        if (!LittleFS.begin(true))
        {
            return;
        }

        _server.on("/api/state", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
                       String state = trafficStateName(_controller->getCurrentState());
                       request->send(200, "application/json", "{\"state\":\"" + state + "\"}"); });

        _server.on("/api/crossing", HTTP_POST, [this](AsyncWebServerRequest *request)
                   {
                       _controller->startPedestrianCrossing();
                       request->send(200, "text/plain", "Pedestrian crossing started"); });

        _server.on("/state", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
                       String state = trafficStateName(_controller->getCurrentState());
                       request->send(200, "application/json", "{\"state\":\"" + state + "\"}"); });

        _server.on("/crossing", HTTP_POST, [this](AsyncWebServerRequest *request)
                   {
                       _controller->startPedestrianCrossing();
                       request->send(200, "text/plain", "Pedestrian crossing started"); });

        _server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

        _server.begin();
    }

    private:
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