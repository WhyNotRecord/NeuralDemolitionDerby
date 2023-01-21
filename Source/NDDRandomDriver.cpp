/*
  ==============================================================================

    NDDRandomDriver.cpp
    Created: 19 Jan 2023 5:57:42pm
    Author:  whyno

  ==============================================================================
*/

#include "NDDRandomDriver.h"

NDDRandomDriver::NDDRandomDriver(NDDCar* _car) : NDDAbstractDriver(_car), r()
{
}

void NDDRandomDriver::update(juce::Rectangle<int> bounds)
{
    if (r.nextFloat() > 0.3f)
        car->accelerate(0.1);
    if (bounds.reduced(50).contains(car->getPosition().x, car->getPosition().y)) {
        float steer = r.nextFloat();
        if (steer < 0.4f)
            car->steer(0.2f - steer);
    }
    else {
        car->steer(1);
    }
}
