/*
  ==============================================================================

    NDDCar.cpp
    Created: 11 Jan 2023 3:43:25pm
    Author:  whyno

  ==============================================================================
*/

#include "NDDCar.h"
#include <math.h>

const float minAcceleration = 0.1f;
const float maxDeceleration = -1.0f;
const float maxSteering = 0.05f;
const float naturalDeceleration = 0.97f;
const float naturalSteeringLoss = 0.95f;
const float gasStopValue = 0.0099f;//must be less than sqrt(minAcceleration)
const float steerStopValue = 0.0001f;
const float reverseSteeringFactor = 2;
const float minExtImpulseAffect = 0.05f;


const int width = 15;
const int length = 30;
const int hWidth = width / 2;
const int hLength = length / 2;

juce::Rectangle<int> bodyBox(-hLength, -hWidth, length, width);

NDDCar::NDDCar(float _x, float _y, int _mass, float _acceleration) :
    NDDPhysicalObject(_x, _y), health(100), gas(0), steering(0), direction(0), mass(_mass), maxAcceleration(_acceleration), id("1"),
    moved(true), extImpulse(0, 0)
{

}

void NDDCar::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::black);
    juce::Line<float> lWheel(3, 0, - 3, 0);//по X сдвигаем на hLength - 7, по Y - на -hWidth
    lWheel.applyTransform(juce::AffineTransform::rotation(steering * 5));
    lWheel.applyTransform(juce::AffineTransform::translation(juce::Point<float>(hLength - 7, - hWidth)));
    lWheel.applyTransform(juce::AffineTransform::rotation(direction));
    lWheel.applyTransform(juce::AffineTransform::translation(position));
    g.drawLine(lWheel);
    juce::Line<float> rWheel(3, 0, - 3, 0);//по X сдвигаем на hLength - 7, по Y - на hWidth + 1
    rWheel.applyTransform(juce::AffineTransform::rotation(steering * 5));
    rWheel.applyTransform(juce::AffineTransform::translation(juce::Point<float>(hLength - 7, hWidth + 1)));
    rWheel.applyTransform(juce::AffineTransform::rotation(direction));
    rWheel.applyTransform(juce::AffineTransform::translation(position));
    g.drawLine(rWheel);

    g.setColour(juce::Colours::grey);
    g.fillPath(body);

    //g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    g.drawText(id, body.getBounds(), juce::Justification::centred, true);   // draw some placeholder text
}

void NDDCar::update() {
    if (steering != 0 && gas != 0) {
        rotate((gas > 0 ? steering : reverseSteeringFactor * steering) * gas / maxAcceleration);
        if (steering * steering < steerStopValue)
            steering = 0;
        else
            steering *= naturalSteeringLoss;
    }
    if (moved) {
        body = juce::Path();
        body.addRectangle(bodyBox);
        body.applyTransform(juce::AffineTransform::rotation(direction));
        body.applyTransform(juce::AffineTransform::translation(position));
        moved = false;
    }
    if (gas != 0) {
        juce::Point<float> p(gas, 0);
        p.applyTransform(juce::AffineTransform::rotation(direction));
        /*xCord += p.x;
        yCord += p.y;*/
        position += p;
        if (gas * gas < gasStopValue)
            gas = 0;
        else
            gas *= naturalDeceleration;
        moved = true;
    }
    if (!extImpulse.isOrigin()) {
        position += extImpulse;
        if (fabsf(extImpulse.x) + fabsf(extImpulse.y) < gasStopValue)
            extImpulse = juce::Point<float>(0, 0);
        else {
            float cos2 = powf(cosf(direction) * extImpulse.x + sinf(direction) * extImpulse.y, 2);
            cos2 /= (powf(extImpulse.x, 2) + powf(extImpulse.y, 2));
            juce::Logger::writeToLog(juce::String(cos2));
            float factor = 0.45f * (sqrtf(cos2) + 1);//немного "размываем" действие косинуса угла
            extImpulse *= (factor);
        }
        moved = true;
    }
}

void NDDCar::setPosition(float _x, float _y) {
    NDDPhysicalObject::setPosition(_x, _y);
    moved = true;
}

/*float NDDCar::getGas() const
{
    return gas;
}

void NDDCar::setGas(float gas)
{
    this->gas = gas;
}*/

void NDDCar::accelerate(float value)//TODO can make acceleration step NDDCar member
{
    if (value < minAcceleration)
        value = minAcceleration;
    this->gas += value;
    if (this->gas > maxAcceleration)
        this->gas = maxAcceleration;
}

void NDDCar::decelerate(float value)
{
    if (value < minAcceleration)
        value = minAcceleration;
    this->gas -= value;
    if (this->gas < maxDeceleration)
        this->gas = maxDeceleration;
}



float NDDCar::getDirection() const
{
    return direction;
}

void NDDCar::setDirection(float direction)
{
    this->direction = direction;
    moved = true;
}

void NDDCar::rotate(float value) {
    this->direction += value;
    moved = true;
}

void NDDCar::steer(float value) {
    this->steering += value;
    if (this->steering > maxSteering) {
        this->steering = maxSteering;
    } else if (this->steering < -maxSteering) {
        this->steering = -maxSteering;
    }
}


int NDDCar::getHealth() const
{
    return health;
}

void NDDCar::setHealth(int health)
{
    this->health = health;
}

void NDDCar::decrementHealth(int value)
{
    this->health -= value;
    if (health < 0)
        health = -1;
}


int NDDCar::getMass() const
{
    return mass;
}

/*void NDDCar::setMass(int mass)
{
    this->mass = mass;
}*/

/*float NDDCar::getMaxAcceleration() const
{
    return maxAcceleration;
}

void NDDCar::setMaxAcceleration(float maxAcceleration)
{
    this->maxAcceleration = maxAcceleration;
}*/

juce::Point<float> NDDCar::getImpulse() {
    if (gas == 0)
        return extImpulse;

    juce::Point<float> result(gas, 0);
    result.applyTransform(juce::AffineTransform::rotation(direction));
    return result + extImpulse;
}

void NDDCar::addImpulse(juce::Point<float> value) {
    extImpulse += value;

    float steeredDirection = direction - steering;
    float leftPart = cosf(steeredDirection) * extImpulse.x,
        rightPart = sinf(steeredDirection) * extImpulse.y;
    float cos2 = powf(leftPart + rightPart, 2);
    cos2 /= (powf(extImpulse.x, 2) + powf(extImpulse.y, 2));
    if (cos2 > minExtImpulseAffect) {
        if (leftPart * rightPart >= 0 && (leftPart > 0 || rightPart > 0))
            accelerate(cos2 / 10.f);
        else
            decelerate(cos2 / 10.f);
    }

}

float NDDCar::getMaxRadius2() {
    if (radius == 0) {
        radius = powf(width, 2) + powf(length, 2);
    }
    return radius;
}

juce::Path *NDDCar::getBounds() {
    return &body;
}

juce::String NDDCar::getId() const
{
    return id;
}

void NDDCar::setId(juce::String id)
{
    this->id = id;
}

