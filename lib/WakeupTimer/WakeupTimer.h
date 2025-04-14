#ifndef WAKEUP_TIMER_H
#define WAKEUP_TIMER_H

#include <Arduino.h>

class WakeupTimer {
public:
    void enablePeriodicWake(uint64_t interval_us);
};

#endif
