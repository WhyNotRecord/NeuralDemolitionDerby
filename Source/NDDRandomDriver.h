/*
  ==============================================================================

    NDDRandomDriver.h
    Created: 19 Jan 2023 5:57:42pm
    Author:  whyno

  ==============================================================================
*/

#pragma once

#include "NDDCar.h"
#include "NDDAbstractDriver.h"
#include <JuceHeader.h>

class NDDRandomDriver : public NDDAbstractDriver {
public:
    NDDRandomDriver(NDDCar* _car);
    void update(juce::Rectangle<int> bounds) override;

private:
    juce::Random r;
};