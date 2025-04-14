#include "SleepManager.h"

RTC_DATA_ATTR int wakeCounter = 0;

void SleepManager::handleWakeup() {
    wakeCounter++;
    Serial.println("==== Wake-up Event Detected ====");
    Serial.print("[Wake] Wake counter: ");
    Serial.println(wakeCounter);
    printWakeReason();
}

void SleepManager::printWakeReason() {
    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    Serial.print("[Wake] Wake-up reason: ");
    switch (reason) {
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("Timer (2-minute timeout)");
            break;
        case ESP_SLEEP_WAKEUP_UNDEFINED:
            Serial.println("Power-on or reset");
            break;
        default:
            Serial.println("Other wake-up source");
    }
}

void SleepManager::goToSleep() {
    Serial.println("[Sleep] Preparing to sleep for 2 minutes...");
    esp_sleep_enable_timer_wakeup(2 * 60 * 1000000); 
    Serial.println("[Sleep] Going to deep sleep now. See you later 💤");
    delay(100);
    esp_deep_sleep_start();
}
