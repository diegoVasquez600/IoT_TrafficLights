#pragma once
#include <WiFi.h>

class WiFiService
{
private:
    const char *_ssid;
    const char *_password;
public:
    WiFiService(const char *ssid, const char *password) : _ssid(ssid), _password(password) {}
    void begin()
    {
        WiFi.begin(_ssid, _password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nConnected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    void disconnect()
    {
        WiFi.disconnect();
        Serial.println("Disconnected from WiFi");
    }
    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }
    String getIPAddress()
    {
        if (isConnected())
        {
            return WiFi.localIP().toString();
        }
        else
        {
            return String("Not connected");
        }
    }
    String getSSID()
    {
        if (isConnected())
        {
            return WiFi.SSID();
        }
        else
        {
            return String("Not connected");
        }
    }
    String getMACAddress()
    {
        if (isConnected())
        {
            return WiFi.macAddress();
        }
        else
        {
            return String("Not connected");
        }
    }
    int32_t getRSSI()
    {
        if (isConnected())
        {
            return WiFi.RSSI();
        }
        else
        {
            return 0;
        }
    }
    int32_t getChannel()
    {
        if (isConnected())
        {
            return WiFi.channel();
        }
        else
        {
            return 0;
        }
    }
    wifi_auth_mode_t getEncryptionType()
    {
        if (isConnected())
        {
            return WiFi.encryptionType(0);
        }
        else
        {
            return WIFI_AUTH_OPEN;
        }
    }
};