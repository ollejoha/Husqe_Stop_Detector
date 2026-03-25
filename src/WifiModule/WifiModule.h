#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <Arduino.h>

namespace WiFiModule
{
    void begin(const char* ssid, const char* password);
    void connect();
    void disconnect();
    void update();

    bool isConnected();
    bool isConnecting();
}
#endif