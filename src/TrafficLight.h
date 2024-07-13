#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <Arduino.h>

typedef enum State { RED, YELLOW, GREEN } LedState;

class TrafficLight {
public:
    TrafficLight();

    void setRedDuration(unsigned long duration);
    void setYellowDuration(unsigned long duration);
    void setGreenDuration(unsigned long duration);
    void setRedNextDuration(unsigned long duration);
    void setYellowNextDuration(unsigned long duration);
    void setGreenNextDuration(unsigned long duration);
    void init(LedState state);
    void updateState();

    LedState getState();
    unsigned long getRemainingTime();

private:
    unsigned long redDuration, yellowDuration, greenDuration;
    unsigned long redNextDuration = 0, yellowNextDuration = 0, greenNextDuration = 0;
    unsigned long previousMillis;
    LedState currentState;

    void changeState(LedState newState);
};

#endif
