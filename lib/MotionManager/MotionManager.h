#ifndef MOTION_MANAGER_H
#define MOTION_MANAGER_H

#include <Arduino.h>

class MotionManager {
private:
    void (*motionCallback)();

public:
    void begin();
    void setCallback(void (*callback)());
    void simulate(); // simulate motion detection loop
};

#endif
