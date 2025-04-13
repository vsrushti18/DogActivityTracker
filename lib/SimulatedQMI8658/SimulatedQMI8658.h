#ifndef SIMULATED_QMI8658_H
#define SIMULATED_QMI8658_H

struct IMUdata {
    float x;
    float y;
    float z;
};

class SimulatedQMI8658 {
private:
    bool motionDetected;
    void (*wakeupCallback)();

public:
    SimulatedQMI8658();

    bool begin();
    bool readFromFifo(IMUdata* acc, int accCount, IMUdata* gyr, int gyrCount);
    void configWakeOnMotion();
    void setWakeupMotionEventCallBack(void (*callback)());
    void simulateMotion();
};

#endif
