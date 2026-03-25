#ifndef VIBRATION_MODULE_H
#define VIBRATION_MODULE_H

#include <Arduino.h>

namespace VibrationSensorModule
{
    void begin(uint8_t pin, unsigned long timeoutMs);
    void update();

    bool isMoving();
    bool isSensorActive();
    unsigned long getLastMotionTime();
}

#endif