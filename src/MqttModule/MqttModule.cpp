#include "MqttModule.h"
#include <WiFi.h>
#include <PubSubClient.h>

namespace MqttModule
{
    namespace
    {
        WiFiClient wifiClient;
        PubSubClient mqttClient(wifiClient);

        const char* mqttBroker = nullptr;
        uint16_t mqttPort = 1883;
        const char* mqttClientId = nullptr;
    }

    void begin(const char* broker, uint16_t port, const char* clientId)
    {
        mqttBroker = broker;
        mqttPort = port;
        mqttClientId = clientId;

        mqttClient.setServer(mqttBroker, mqttPort);
    }

    bool publishStopMessage(const char* topic, const char* payload)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("MQTT kan inte skickas, WiFi saknas.");
            return false;
        }

        Serial.println("Ansluter till MQTT broker...");

        if (!mqttClient.connect(mqttClientId))
        {
            Serial.print("MQTT connect failed, rc=");
            Serial.println(mqttClient.state());
            return false;
        }

        bool ok = mqttClient.publish(topic, payload, true);

        if (!ok)
        {
            Serial.println("MQTT publish misslyckades.");
        }

        mqttClient.disconnect();
        return ok;
    }
}