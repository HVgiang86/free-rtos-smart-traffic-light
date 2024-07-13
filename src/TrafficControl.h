#ifndef TRAFFICCONTROL_H
#define TRAFFICCONTROL_H

#include "TrafficLight.h"

typedef enum TrafficState { NORTH_SOUTH_GREEN, NORTH_SOUTH_YELLOW, EAST_WEST_GREEN, EAST_WEST_YELLOW } TrafficState;

class TrafficControl {
public:
    TrafficControl(TrafficLight& lightA, TrafficLight& lightB);

    void updateDuration(unsigned long lightARed, unsigned long lightBRed);

    void init();
    void update();

private:
    TrafficLight& lightA;
    TrafficLight& lightB;

};

#endif
