/*
  ==============================================================================

    NDDAbstractDriver.h
    Created: 20 Jan 2023 2:57:05pm
    Author:  whyno

  ==============================================================================
*/

#pragma once

#include "NDDCar.h"
#include <JuceHeader.h>

class NDDAbstractDriver {
public:
    NDDAbstractDriver(NDDCar* _car);
    virtual void update(juce::Rectangle<int> bounds) = 0;

protected:
    NDDCar* car;
};