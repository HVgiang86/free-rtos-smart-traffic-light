#include "TrafficLight.h"

TrafficLight::TrafficLight()
{
}

void TrafficLight::setRedDuration(unsigned long duration)
{
    redDuration = duration;
}

void TrafficLight::setYellowDuration(unsigned long duration)
{
    yellowDuration = duration;
}

void TrafficLight::setGreenDuration(unsigned long duration)
{
    greenDuration = duration;
}

void TrafficLight::setRedNextDuration(unsigned long duration)
{
    redNextDuration = duration;
}

void TrafficLight::setYellowNextDuration(unsigned long duration)
{
    yellowNextDuration = duration;
}

void TrafficLight::setGreenNextDuration(unsigned long duration)
{
    greenNextDuration = duration;
}

void TrafficLight::init(LedState state)
{
    currentState = state;
    previousMillis = millis();
}

void TrafficLight::updateState()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= (currentState == RED ? redDuration : currentState == YELLOW ? yellowDuration
                                                                                                      : greenDuration))
    {
        previousMillis = currentMillis;
        switch (currentState)
        {
        case RED:
            changeState(GREEN);
            break;
        case GREEN:
            changeState(YELLOW);
            break;
        case YELLOW:
            changeState(RED);
            break;
        }
    }
}

LedState TrafficLight::getState()
{
    return currentState;
}

unsigned long TrafficLight::getRemainingTime()
{
    unsigned long currentMillis = millis();
    unsigned long elapsedTime = currentMillis - previousMillis;
    unsigned long duration = (currentState == RED ? redDuration : currentState == YELLOW ? yellowDuration
                                                                                         : greenDuration);

    if (elapsedTime >= duration)
    {
        return 0;
    }
    else
    {
        return duration - elapsedTime;
    }
}

void TrafficLight::changeState(LedState newState)
{
    currentState = newState;
    previousMillis = millis();

    if (redNextDuration != 0)
    {
        redDuration = redNextDuration;
        redNextDuration = 0;
    }

    if (greenNextDuration != 0)
    {
        greenDuration = greenNextDuration;
        greenNextDuration = 0;
    }
    if (yellowNextDuration != 0)
    {
        yellowDuration = yellowNextDuration;
        yellowNextDuration = 0;
    }
}
