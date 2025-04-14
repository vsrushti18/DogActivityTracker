#include "TimeManager.h"

RTC_DATA_ATTR time_t lastSyncTime = 0;

const char* ssid = "yourSSID";         // Replace with actual credentials
const char* password = "yourPASSWORD"; // Replace with actual credentials
const long gmtOffset_sec = 19800;      // GMT +5:30 for IST
const int daylightOffset_sec = 0;

void TimeManager::begin() {
    connectToWiFi();
    syncWithNTP();
}

void TimeManager::connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("[WiFi] Connecting");
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
    } else {
        Serial.println("\n[WiFi] Failed to connect");
    }
}

void TimeManager::syncWithNTP() {
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        lastSyncTime = time(NULL);
        Serial.print("[NTP] Time synced: ");
        Serial.println(getCurrentTimeString());
    } else {
        Serial.println("[NTP] Failed to obtain time");
    }
}

String TimeManager::getCurrentTimeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "Time not available";
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

String TimeManager::getLastSyncTimeString() {
    if (lastSyncTime == 0) return "Never synced";
    char buffer[30];
    struct tm* timeinfo = localtime(&lastSyncTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return String(buffer);
}
