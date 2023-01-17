#pragma once

#include <JuceHeader.h>
#include "NDDCar.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void update() override;
    void createCar(float xPos, float yPos, int mass, float acceleration, juce::String id);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& k) override;
    bool keyStateChanged(bool isKeyDown) override;
    bool processKeysState();

private:
    //==============================================================================
    // Your private member variables go here...
    NDDCar userCar, enemyCar;
    std::vector<NDDCar*> cars;
    juce::String collision;
    //int keysPressed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
