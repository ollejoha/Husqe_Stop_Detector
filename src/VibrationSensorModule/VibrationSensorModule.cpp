#include "VibrationSensorModule.h"

namespace VibrationSensorModule
{
    namespace
    {
        uint8_t vibrationPin = 255;
        unsigned long motionTimeoutMs = 30000;

        bool mowerMoving = false;
        bool sensorActive = false;
        bool moduleInitialized = false;

        unsigned long lastMotionTime = 0;
    }

    void begin(uint8_t pin, unsigned long timeoutMs)
    {
        vibrationPin = pin;
        motionTimeoutMs = timeoutMs;

        pinMode(vibrationPin, INPUT);

        mowerMoving = false;
        sensorActive = false;
        lastMotionTime = 0;
        moduleInitialized = true;
    }

    void update()
    {
        if (!moduleInitialized)
        {
            return;
        }

        int rawState = digitalRead(vibrationPin);

        // Anpassa här om sensorn är inverterad
        sensorActive = (rawState == HIGH);

        if (sensorActive)
        {
            lastMotionTime = millis();
            mowerMoving = true;
        }

        if (mowerMoving && (millis() - lastMotionTime > motionTimeoutMs))
        {
            mowerMoving = false;
        }
    }

    bool isMoving()
    {
        return mowerMoving;
    }

    bool isSensorActive()
    {
        return sensorActive;
    }

    unsigned long getLastMotionTime()
    {
        return lastMotionTime;
    }
}