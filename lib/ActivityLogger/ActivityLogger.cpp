#include "ActivityLogger.h"
#include <time.h>

void ActivityLogger::begin() {
    if (!LittleFS.begin()) {
        Serial.println("[FS] LittleFS Mount Failed!");
        return;
    }
    Serial.println("[FS] LittleFS Mounted.");
    lastLogTime = millis();
}

void ActivityLogger::addActivity(ActivityType activity) {
    switch (activity) {
        case RESTING: restSecs += 2; break;
        case WALKING: walkSecs += 2; break;
        case RUNNING: runSecs += 2; break;
        case PLAYING: playSecs += 2; break;
        default: break;
    }
}

void ActivityLogger::logToFile() {
    if (millis() - lastLogTime >= 600000) {
        uint32_t timestamp = time(NULL);
        writeBinaryLog(timestamp);

        Serial.printf("[LOGGED] ts=%lu | R=%u W=%u Rn=%u P=%u\n",
            timestamp, restSecs, walkSecs, runSecs, playSecs);

        restSecs = walkSecs = runSecs = playSecs = 0;
        lastLogTime = millis();
    }
}

String ActivityLogger::getDailyFilename() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "/data/default.dat";

    char buffer[32];
    strftime(buffer, sizeof(buffer), "/data/%Y%m%d.dat", &timeinfo);
    return String(buffer);
}

void ActivityLogger::writeBinaryLog(uint32_t timestamp) {
    String filename = getDailyFilename();
    File file = LittleFS.open(filename, FILE_APPEND);

    if (!file) {
        Serial.println("[FS] Failed to open file!");
        return;
    }

    file.write((uint8_t*)&timestamp, sizeof(timestamp));
    file.write((uint8_t*)&restSecs, sizeof(restSecs));
    file.write((uint8_t*)&walkSecs, sizeof(walkSecs));
    file.write((uint8_t*)&runSecs, sizeof(runSecs));
    file.write((uint8_t*)&playSecs, sizeof(playSecs));
    
    file.close();
}
