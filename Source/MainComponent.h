#pragma once

#include <JuceHeader.h>
#include "NDDCar.h"
#include "NDDStraightDriver.h"

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
    NDDCar *createCar(float xPos, float yPos, int mass, float acceleration, juce::String id);
    void validatePossibleCollision(NDDCar* c1, NDDCar* c2);
    void processCollision(NDDCar* fastCar, NDDCar* slowCar,
        juce::Point<float> fastCarImp, juce::Point<float> slowCarImp, juce::Point<float>* cp);
    juce::Point<float>* getCollisionPoint(juce::Path* c1Bounds, juce::Point<float> imp1, 
        juce::Path* c2Bounds, juce::Point<float> imp2);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& k) override;
    bool keyStateChanged(bool isKeyDown) override;
    bool processKeysState();

private:
    //==============================================================================
    // Your private member variables go here...
    NDDCar userCar;
    std::vector<NDDCar*> cars;
    std::map<juce::String, int> actCollisions;
    juce::String collision;
    NDDStraightDriver rDriver = NULL;
    //int keysPressed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
