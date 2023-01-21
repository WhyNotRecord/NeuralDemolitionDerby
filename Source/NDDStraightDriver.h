/*
  ==============================================================================

    NDDStraightDriver.h
    Created: 20 Jan 2023 2:57:15pm
    Author:  whyno

  ==============================================================================
*/

#pragma once

#include "NDDCar.h"
#include "NDDAbstractDriver.h"
#include <JuceHeader.h>

class NDDStraightDriver : public NDDAbstractDriver {
public:
    NDDStraightDriver(NDDCar* _car);
    void update(juce::Rectangle<int> bounds) override;

private:
    juce::Random r;
};