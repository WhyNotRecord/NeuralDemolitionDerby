/*
  ==============================================================================

    NDDPhysicalObject.cpp
    Created: 11 Jan 2023 3:44:24pm
    Author:  whyno

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NDDPhysicalObject.h"

//==============================================================================
NDDPhysicalObject::NDDPhysicalObject(float _x, float _y) : 
    position(_x, _y), impulse(0, 0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    /*xCord = _x;
    yCord = _y;*/
}

NDDPhysicalObject::~NDDPhysicalObject()
{
}

void NDDPhysicalObject::update() {
}


void NDDPhysicalObject::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void NDDPhysicalObject::setPosition(float _x, float _y) {
    position.setXY(_x, _y);
}

juce::Point<float> NDDPhysicalObject::getPosition() {
    return position;
}

