#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

class TimeManager {
public:
    void begin();
    void syncWithNTP();
    String getCurrentTimeString();
    String getLastSyncTimeString();

private:
    void connectToWiFi();
};

#endif