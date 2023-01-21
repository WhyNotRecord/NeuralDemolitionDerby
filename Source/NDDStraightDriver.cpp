/*
  ==============================================================================

    NDDStraightDriver.cpp
    Created: 20 Jan 2023 2:57:15pm
    Author:  whyno

  ==============================================================================
*/

#include "NDDStraightDriver.h"

NDDStraightDriver::NDDStraightDriver(NDDCar* _car) : NDDAbstractDriver(_car), r()
{
}

void NDDStraightDriver::update(juce::Rectangle<int> bounds)
{
    if (r.nextFloat() > 0.3f)
        car->accelerate(0.1);
    if (!bounds.reduced(75).contains(car->getPosition().x, car->getPosition().y)) {
        juce::Point<float> p = car->getImpulse();
        car->steer(p.x * p.x - 2 * p.x * p.y + p.y * p.y);

        /*float leftPart = cosf(car->getDirection()) * p.x,
            rightPart = sinf(car->getDirection()) * p.y;
        float cos2 = powf(leftPart + rightPart, 2);
        cos2 /= (powf(extImpulse.x, 2) + powf(extImpulse.y, 2));*/

    }
}