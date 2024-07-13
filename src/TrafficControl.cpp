#include "TrafficControl.h"

TrafficControl::TrafficControl(TrafficLight &lightA, TrafficLight &lightB)
    : lightA(lightA), lightB(lightB) {}

void TrafficControl::updateDuration(unsigned long lightARed, unsigned long lightBRed)
{
    if (lightA.getState() == GREEN || lightA.getState() == YELLOW)
    {
        // Serial.println("Trigger A");
        lightA.setRedNextDuration(lightARed);
        lightA.setGreenNextDuration(lightARed - 3000);

        lightB.setRedNextDuration(lightARed);
        lightB.setGreenNextDuration(lightARed - 3000);
        
    } else if (lightA.getState() == GREEN || lightB.getState() == YELLOW) {
        // Serial.println("Trigger B");

        lightA.setRedNextDuration(lightBRed);
        lightA.setGreenNextDuration(lightBRed - 3000);

        lightB.setRedNextDuration(lightBRed);
        lightB.setGreenNextDuration(lightBRed - 3000);
    }
}

void TrafficControl::init()
{
    lightA.setRedDuration(60000);
    lightA.setGreenDuration(57000);
    lightA.setYellowDuration(3000);

    lightB.setRedDuration(60000);
    lightB.setGreenDuration(57000);
    lightB.setYellowDuration(3000);

    lightA.init(RED);
    lightB.init(GREEN);
}

void TrafficControl::update()
{
    lightA.updateState();
    lightB.updateState();
}
