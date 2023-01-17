/*
  ==============================================================================

    NDDPhysicalObject.h
    Created: 11 Jan 2023 3:44:24pm
    Author:  whyno

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NDDPhysicalObject  : public juce::Component
{
public:
    NDDPhysicalObject(float _x, float _y);
    ~NDDPhysicalObject() override;

    //void paint (juce::Graphics&) override;
    virtual void update();
    void resized() override;

    virtual void setPosition(float _x, float _y);
    virtual juce::Point<float> getPosition();

private:

protected:
    float xCord, yCord;
    juce::Point<float> position;
    juce::Point<float> impulse;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NDDPhysicalObject)
};
