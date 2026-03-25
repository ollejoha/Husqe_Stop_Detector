#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include <Arduino.h>

namespace MqttModule
{
    void begin(const char* broker, uint16_t port, const char* clientId);
    bool publishStopMessage(const char* topic, const char* payload);
}

#endif