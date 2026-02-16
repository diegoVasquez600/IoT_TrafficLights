#pragma once
#include <Arduino.h>


class WebPageBuilder
{
    public:
    static String buildDashboardPage(const String& trafficState, const String& ipAddress)
    {
        String html = "<!DOCTYPE html><html><head><title>Traffic Light Dashboard</title><style>";
        html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0; }";
        html += ".container { margin-top: 50px; }";
        html += ".state { font-size: 24px; margin-bottom: 20px; }";
        html += ".ip { font-size: 18px; color: #555; }";
        html += "</style></head><body><div class='container'>";
        html += "<div class='state'>Current Traffic State: <strong>" + trafficState + "</strong></div>";
        html += "<div class='ip'>Device IP Address: " + ipAddress + "</div>";
        html += "</div></body></html>";
        return html;
    }
};