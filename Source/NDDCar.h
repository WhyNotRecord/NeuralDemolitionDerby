/*
  ==============================================================================

    NDDCar.h
    Created: 11 Jan 2023 3:43:25pm
    Author:  whyno

  ==============================================================================
*/

#pragma once

#include "NDDPhysicalObject.h"

class NDDCar : public NDDPhysicalObject {
public:
    NDDCar(float _x, float _y, int _mass, float _acceleration);

    void paint(juce::Graphics&) override;
    void update() override;

    virtual void setPosition(float _x, float _y);

private:
    juce::Path body;
    bool moved;
    float gas;
    float direction;
    float steering;
    float rotation;
    int health;
    int mass;
    float maxAcceleration;
    juce::Point<float> extImpulse;
    float radius = 0;
    uint16_t stunned;
    juce::String id;
public:
    /*float getGas() const;
    void setGas(float gas);*/
    void accelerate(float value);
    void decelerate(float value);
    void brake(float value);
    void stop();

    float getDirection() const;
    void setDirection(float direction);
    void rotate(float value);
    void steer(float value);
    void startRotation(float value);

    int getHealth() const;
    void setHealth(int health);
    void decrementHealth(int value);

    int getMass() const;
    //void setMass(int mass);

    /*float getMaxAcceleration() const;
    void setMaxAcceleration(float maxAcceleration);*/

    juce::Point<float> getImpulse();
    void addImpulse(juce::Point<float> value);
    void setImpulse(juce::Point<float> value);

    float getMaxRadius2();
    juce::Path *getBounds();
    void setStunned(uint16_t value);

    juce::String getId() const;
    void setId(juce::String id);

    int getWidth();
    int getLength();
};