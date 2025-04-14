#include <Arduino.h>
#include "SimulatedQMI8658.h"
#include "MotionManager.h"
#include "ActivityClassifier.h"
#include "ActivityLogger.h"
#include "TimeManager.h"
#include "WakeupTimer.h"

// 👇 Uncomment this to enable simulation mode without hardware
#define TEST_MODE

RTC_DATA_ATTR int wakeCounter = 0;

SimulatedQMI8658 sensor;
MotionManager motion;
ActivityClassifier classifier;
ActivityLogger logger;
TimeManager timeMgr;
WakeupTimer timerMgr;

IMUdata acc[1];
IMUdata gyr[1];

void onMotionDetected() {
    Serial.println(">> Motion callback triggered!");

    sensor.readFromFifo(acc, 1, gyr, 1);

    ActivityType activity = classifier.classify(acc[0], gyr[0]);
    logger.addActivity(activity);

    Serial.print("Accel: ");
    Serial.print(acc[0].x); Serial.print(", ");
    Serial.print(acc[0].y); Serial.print(", ");
    Serial.println(acc[0].z);

    Serial.print("Gyro: ");
    Serial.print(gyr[0].x); Serial.print(", ");
    Serial.print(gyr[0].y); Serial.print(", ");
    Serial.println(gyr[0].z);

    Serial.print("[Classified] ");
    Serial.println(classifier.activityToString(activity));
    Serial.println("--------------------------");
}

void goToSleep() {
    Serial.println("[Sleep] Preparing to sleep for 2 minutes...");
    esp_sleep_enable_timer_wakeup(2 * 60 * 1000000ULL); // 2 mins
    Serial.println("[Sleep] Going to deep sleep now. See you later 💤");
    delay(100);
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    wakeCounter++;
    Serial.println("==== Wake-up Event Detected ====");
    Serial.print("[Wake] Wake counter: ");
    Serial.println(wakeCounter);

    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    Serial.print("[Wake] Wake-up reason: ");
    switch (reason) {
        case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Timer (2-minute timeout)"); break;
        case ESP_SLEEP_WAKEUP_UNDEFINED: Serial.println("Power-on or reset"); break;
        default: Serial.println("Other wake-up source"); break;
    }

    sensor.begin();
    sensor.configWakeOnMotion();
    sensor.setWakeupMotionEventCallBack(onMotionDetected);

    motion.begin();
    motion.setCallback(onMotionDetected);

    classifier = ActivityClassifier();
    logger.begin();
    timeMgr.begin();

    timerMgr.enablePeriodicWake(2 * 60 * 1000000ULL);

    Serial.print("[RTC] Current Time: ");
    Serial.println(timeMgr.getCurrentTimeString());

#ifdef TEST_MODE
    Serial.println("[TEST_MODE] Enabled. Motion will be simulated every cycle.");
#endif
}

void loop() {
#ifdef TEST_MODE
    // Simulate a motion event every time in test mode
    Serial.println("[TEST_MODE] Simulating motion now...");
    onMotionDetected();
#else
    // Normal behavior
    sensor.simulateMotion();
    motion.simulate();
#endif

    logger.logToFile();

    Serial.println("[Loop] Cycle complete.\n");
    delay(2000); // Wait before next cycle

    // goToSleep(); // ← Only for actual deep sleep test
}
