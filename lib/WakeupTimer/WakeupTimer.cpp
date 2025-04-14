#include "WakeupTimer.h"

void WakeupTimer::enablePeriodicWake(uint64_t interval_us) {
    Serial.printf("[Timer] Setting up wake every %llu microseconds...\n", interval_us);
    esp_sleep_enable_timer_wakeup(interval_us);
}
