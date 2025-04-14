#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <Arduino.h>

class SleepManager {
public:
    void handleWakeup();   
    void goToSleep();    

private:
    void printWakeReason();
};

#endif
