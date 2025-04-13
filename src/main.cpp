#include <Arduino.h>
#include "SimulatedQMI8658.h"

SimulatedQMI8658 sensor;

IMUdata acc[1];
IMUdata gyr[1];

void onMotionDetected() {
    Serial.println(">> Motion callback triggered!");
    sensor.readFromFifo(acc, 1, gyr, 1);

    Serial.print("Accel: ");
    Serial.print(acc[0].x); Serial.print(", ");
    Serial.print(acc[0].y); Serial.print(", ");
    Serial.println(acc[0].z);

    Serial.print("Gyro: ");
    Serial.print(gyr[0].x); Serial.print(", ");
    Serial.print(gyr[0].y); Serial.print(", ");
    Serial.println(gyr[0].z);
}

void setup() {
    Serial.begin(115200);
    delay(1000); // Let Serial wake up
    sensor.begin();
    sensor.configWakeOnMotion();
    sensor.setWakeupMotionEventCallBack(onMotionDetected);
}

void loop() {
    sensor.simulateMotion(); // randomly simulates motion
    delay(500);
}
