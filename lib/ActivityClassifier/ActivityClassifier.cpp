#include "ActivityClassifier.h"

ActivityType ActivityClassifier::classify(const IMUdata& acc, const IMUdata& gyr) {
    float accMag = sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    float gyrMag = sqrt(gyr.x * gyr.x + gyr.y * gyr.y + gyr.z * gyr.z);

    if (accMag < 1.2 && gyrMag < 10) return RESTING;
    else if (accMag < 2.5 && gyrMag < 50) return WALKING;
    else if (accMag < 5.0 && gyrMag < 100) return RUNNING;
    else return PLAYING;
}

String ActivityClassifier::activityToString(ActivityType activity) {
    switch (activity) {
        case RESTING: return "Resting";
        case WALKING: return "Walking";
        case RUNNING: return "Running";
        case PLAYING: return "Playing";
        default: return "Unknown";
    }
}
