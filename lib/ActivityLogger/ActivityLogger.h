#ifndef ACTIVITY_LOGGER_H
#define ACTIVITY_LOGGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include "TimeManager.h"
#include "ActivityClassifier.h"

class ActivityLogger {
public:
    void begin();
    void addActivity(ActivityType activity);
    void logToFile();

private:
    int restSecs = 0;
    int walkSecs = 0;
    int runSecs = 0;
    int playSecs = 0;
    unsigned long lastLogTime = 0;
    TimeManager* timeMgr;

    void writeBinaryLog(uint32_t timestamp);
    String getDailyFilename();
};

#endif
