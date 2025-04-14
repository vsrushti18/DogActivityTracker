#include <Arduino.h>
#include "SimulatedQMI8658.h"
#include "MotionManager.h"
#include "ActivityClassifier.h"
#include "ActivityLogger.h"
#include "TimeManager.h"
#include "WakeupTimer.h"

#define TEST_MODE  // Comment this out to disable simulated motion loop

RTC_DATA_ATTR int wakeCounter = 0;

SimulatedQMI8658 sensor;
MotionManager motion;
ActivityClassifier classifier;
ActivityLogger logger;
TimeManager timeMgr;
WakeupTimer timerMgr;

IMUdata acc[1];
IMUdata gyr[1];

// Use Serial2 for debugging
#define DEBUG_SERIAL Serial2
#define DEBUG_BAUD 115200

void onMotionDetected() {
    DEBUG_SERIAL.println(">> Motion callback triggered!");

    sensor.readFromFifo(acc, 1, gyr, 1);

    ActivityType activity = classifier.classify(acc[0], gyr[0]);
    logger.addActivity(activity);

    DEBUG_SERIAL.print("Accel: ");
    DEBUG_SERIAL.print(acc[0].x); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.print(acc[0].y); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(acc[0].z);

    DEBUG_SERIAL.print("Gyro: ");
    DEBUG_SERIAL.print(gyr[0].x); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.print(gyr[0].y); DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.println(gyr[0].z);

    DEBUG_SERIAL.print("[Classified] ");
    DEBUG_SERIAL.println(classifier.activityToString(activity));
    DEBUG_SERIAL.println("--------------------------");
}

void goToSleep() {
    DEBUG_SERIAL.println("[Sleep] Preparing to sleep for 2 minutes...");
    esp_sleep_enable_timer_wakeup(2 * 60 * 1000000ULL);
    DEBUG_SERIAL.println("[Sleep] Going to deep sleep now. See you later 💤");
    delay(100);
    esp_deep_sleep_start();
}

void setup() {
    DEBUG_SERIAL.begin(DEBUG_BAUD, SERIAL_8N1, 5, 4);  // RX = GPIO5, TX = GPIO4
    delay(1000);

    wakeCounter++;
    DEBUG_SERIAL.println("==== Wake-up Event Detected ====");
    DEBUG_SERIAL.print("[Wake] Wake counter: ");
    DEBUG_SERIAL.println(wakeCounter);

    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    DEBUG_SERIAL.print("[Wake] Wake-up reason: ");
    switch (reason) {
        case ESP_SLEEP_WAKEUP_TIMER: DEBUG_SERIAL.println("Timer (2-minute timeout)"); break;
        case ESP_SLEEP_WAKEUP_UNDEFINED: DEBUG_SERIAL.println("Power-on or reset"); break;
        default: DEBUG_SERIAL.println("Other wake-up source"); break;
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

    DEBUG_SERIAL.print("[RTC] Current Time: ");
    DEBUG_SERIAL.println(timeMgr.getCurrentTimeString());
}

void loop() {
#ifdef TEST_MODE
    sensor.simulateMotion();
    motion.simulate();
#endif

    logger.logToFile();

    delay(2000);

    // goToSleep(); // ← enable if using actual deep sleep
}
