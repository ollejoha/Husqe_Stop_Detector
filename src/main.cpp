#include <Arduino.h>
#include "VibrationSensorModule/VibrationSensorModule.h"
#include "WiFiModule/WiFiModule.h"
#include "MqttModule/MqttModule.h"

namespace MainApp
{
    constexpr uint8_t VIBRATION_PIN = 10;
    constexpr unsigned long MOTION_TIMEOUT_MS = 30000;

    constexpr char WIFI_SSID[] = "dlink-D69C";
    constexpr char WIFI_PASSWORD[] = "jeggp68439";

    constexpr char MQTT_BROKER[] = "192.168.50.137";
    constexpr uint16_t MQTT_PORT = 1883;
    constexpr char MQTT_CLIENT_ID[] = "mower-stop-detector";
    constexpr char MQTT_TOPIC[] = "mower/status";

    bool lastMotionState = false;
    bool stopReported = false;
    bool wifiConnectStarted = false;
}

void setup()
{
    Serial.begin(115200);
    delay(500);

    Serial.println();
    Serial.println("System startar...");

    VibrationSensorModule::begin(MainApp::VIBRATION_PIN, MainApp::MOTION_TIMEOUT_MS);
    WiFiModule::begin(MainApp::WIFI_SSID, MainApp::WIFI_PASSWORD);
    MqttModule::begin(MainApp::MQTT_BROKER, MainApp::MQTT_PORT, MainApp::MQTT_CLIENT_ID);

    MainApp::lastMotionState = VibrationSensorModule::isMoving();

    Serial.println("Initiering klar.");
}

void loop()
{
    VibrationSensorModule::update();

    bool mowerMoving = VibrationSensorModule::isMoving();

    // Rörelse har startat igen
    if (mowerMoving && !MainApp::lastMotionState)
    {
        Serial.println("Rörelse detekterad igen.");
        MainApp::stopReported = false;
        MainApp::wifiConnectStarted = false;

        WiFiModule::disconnect();
    }

    MainApp::lastMotionState = mowerMoving;

    // Klipparen står still och detta stopp har ännu inte rapporterats
    if (!mowerMoving && !MainApp::stopReported)
    {
        if (!MainApp::wifiConnectStarted)
        {
            Serial.println("Stopp detekterat. Startar WiFi...");
            WiFiModule::connect();
            MainApp::wifiConnectStarted = true;
        }

        WiFiModule::update();

        if (WiFiModule::isConnected())
        {
            Serial.println("WiFi anslutet, skickar MQTT...");

            bool mqttOk = MqttModule::publishStopMessage(MainApp::MQTT_TOPIC, "STOPPED");

            if (mqttOk)
            {
                Serial.println("MQTT skickat OK.");
                MainApp::stopReported = true;
            }
            else
            {
                Serial.println("MQTT misslyckades.");
            }

            WiFiModule::disconnect();
            MainApp::wifiConnectStarted = false;
        }
    }

    delay(10);
}