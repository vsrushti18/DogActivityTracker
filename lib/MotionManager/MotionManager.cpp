#include "MotionManager.h"

void MotionManager::begin() {
    Serial.println("MotionManager initialized");
}

void MotionManager::setCallback(void (*callback)()) {
    motionCallback = callback;
}

void MotionManager::simulate() {
    if (random(100) < 5) { // 5% chance to simulate motion
        Serial.println("MotionManager: Simulated motion detected!");
        if (motionCallback) motionCallback();
    }
}
