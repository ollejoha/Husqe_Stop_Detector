#ifndef POWER_MODULE_H
#define POWER_MODULE_H

#include <Arduino.h>

namespace PowerModule
{
    void begin();
    void lightSleepMs(unsigned long sleepMs);
}

#endif