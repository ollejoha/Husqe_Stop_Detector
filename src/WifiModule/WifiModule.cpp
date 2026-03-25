#include "WiFiModule.h"
#include <WiFi.h>

namespace WiFiModule
{
    namespace
    {
        const char* wifiSsid = nullptr;
        const char* wifiPassword = nullptr;

        bool initialized = false;
        bool connectInProgress = false;
        unsigned long connectStartTime = 0;

        constexpr unsigned long CONNECT_TIMEOUT_MS = 15000;
    }

    void begin(const char* ssid, const char* password)
    {
        wifiSsid = ssid;
        wifiPassword = password;
        initialized = true;

        WiFi.mode(WIFI_OFF);
    }

    void connect()
    {
        if (!initialized)
        {
            return;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            connectInProgress = false;
            return;
        }

        Serial.println("WiFi startas...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiSsid, wifiPassword);

        connectInProgress = true;
        connectStartTime = millis();
    }

    void disconnect()
    {
        connectInProgress = false;

        if (WiFi.getMode() != WIFI_OFF)
        {
            Serial.println("WiFi stängs av...");
            WiFi.disconnect(true, true);
            WiFi.mode(WIFI_OFF);
        }
    }

    void update()
    {
        if (!initialized || !connectInProgress)
        {
            return;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            connectInProgress = false;
            Serial.print("WiFi anslutet. IP: ");
            Serial.println(WiFi.localIP());
            return;
        }

        if (millis() - connectStartTime > CONNECT_TIMEOUT_MS)
        {
            Serial.println("WiFi timeout.");
            disconnect();
        }
    }

    bool isConnected()
    {
        return (WiFi.status() == WL_CONNECTED);
    }

    bool isConnecting()
    {
        return connectInProgress;
    }
}