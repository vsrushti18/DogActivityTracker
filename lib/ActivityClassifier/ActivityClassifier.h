#ifndef ACTIVITY_CLASSIFIER_H
#define ACTIVITY_CLASSIFIER_H

#include <Arduino.h>
#include "SimulatedQMI8658.h"

enum ActivityType {
    RESTING,
    WALKING,
    RUNNING,
    PLAYING
};

class ActivityClassifier {
public:
    ActivityType classify(const IMUdata& acc, const IMUdata& gyr);
    String activityToString(ActivityType activity);
};

#endif
