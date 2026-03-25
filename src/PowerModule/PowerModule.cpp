#include "PowerModule.h"
#include <esp_sleep.h>

namespace PowerModule
{
    void begin()
    {
        // Placeholder för framtida lågströmsinställningar
    }

    void lightSleepMs(unsigned long sleepMs)
    {
        esp_sleep_enable_timer_wakeup((uint64_t)sleepMs * 1000ULL);
        esp_light_sleep_start();
    }
}