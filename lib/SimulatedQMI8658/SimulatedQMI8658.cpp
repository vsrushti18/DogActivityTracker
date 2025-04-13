#include "SimulatedQMI8658.h"
#include <Arduino.h>

SimulatedQMI8658::SimulatedQMI8658() : motionDetected(false), wakeupCallback(nullptr) {}

bool SimulatedQMI8658::begin() {
    Serial.println("SimulatedQMI8658 initialized!");
    return true;
}

bool SimulatedQMI8658::readFromFifo(IMUdata* acc, int accCount, IMUdata* gyr, int gyrCount) {
    for (int i = 0; i < accCount; i++) {
        acc[i].x = random(-32768, 32767) / 1000.0;
        acc[i].y = random(-32768, 32767) / 1000.0;
        acc[i].z = random(-32768, 32767) / 1000.0;
    }

    for (int i = 0; i < gyrCount; i++) {
        gyr[i].x = random(-32768, 32767) / 100.0;
        gyr[i].y = random(-32768, 32767) / 100.0;
        gyr[i].z = random(-32768, 32767) / 100.0;
    }

    return true;
}

void SimulatedQMI8658::configWakeOnMotion() {
    Serial.println("Wake-on-motion configured (simulated)");
}

void SimulatedQMI8658::setWakeupMotionEventCallBack(void (*callback)()) {
    wakeupCallback = callback;
}

void SimulatedQMI8658::simulateMotion() {
    if (random(100) < 5 && !motionDetected) {
        motionDetected = true;
        Serial.println("Simulated motion detected!");
        if (wakeupCallback) wakeupCallback();
    } else {
        motionDetected = false;
    }
}
