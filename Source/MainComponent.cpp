#include "MainComponent.h"

using namespace juce;
//==============================================================================
MainComponent::MainComponent() : 
    userCar(120, 120, 100, 3.f),
    enemyCar(600, 300, 100, 3.5f),
    collision("")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setFramesPerSecond (60); // This sets the frequency of the update calls.
    cars.push_back(&userCar);
    cars.push_back(&enemyCar);
    userCar.setDirection(1.f);
    enemyCar.setId("0");
    enemyCar.setDirection(4.5f);
    enemyCar.accelerate(3);
    enemyCar.steer(-0.5f);

    createCar(500, 500, 100, 3.f, "R");

    setWantsKeyboardFocus(true);
}

MainComponent::~MainComponent()
{
    for (int i = 2; i < cars.size(); i++) {
        delete cars.at(i);
    }
}

//==============================================================================
void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
    processKeysState();
    collision = "";
    for (NDDCar *c1 : cars) {
        c1->update();
        //process collisions
        for (NDDCar* c2 : cars) {
            if (c1 == c2)
                continue;
            float dist2 = c1->getPosition().getDistanceSquaredFrom(c2->getPosition());
            if (dist2 <= c1->getMaxRadius2() + c2->getMaxRadius2()) {
                validatePossibleCollision(c1, c2);
            }
        }
    }
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    for (NDDCar* c : cars) {
        c->paint(g);
    }
    g.setColour(juce::Colours::white);
    g.drawText(collision, 20, 20, 100, 20, juce::Justification::centredLeft);
    // You can add your drawing code here!
}

void MainComponent::createCar(float _x, float _y, int _mass, float _acceleration, juce::String id) {
    NDDCar *newCar = new NDDCar(_x, _y, _mass, _acceleration);
    newCar->setId(id);
    cars.push_back(newCar);
}

void MainComponent::validatePossibleCollision(NDDCar* c1, NDDCar* c2) {
    collision = c1->getId() + "x" + c2->getId();
    juce::Point<float>* cp = getCollisionPoint(c1->getBounds(), c2->getBounds());
    if (cp != NULL) {
        collision += (" - " + cp->toString());
    }
}

juce::Point<float>* MainComponent::getCollisionPoint(juce::Path* c1Bounds, juce::Path* c2Bounds) {
    juce::Path::Iterator it1(*c1Bounds);
    while (it1.next()) {
        if (c2Bounds->contains(juce::Point<float>(it1.x1, it1.y1))) {
            return new juce::Point<float>(it1.x1, it1.y1);
        }
    }
    juce::Path::Iterator it2(*c2Bounds);
    while (it2.next()) {
        if (c1Bounds->contains(juce::Point<float>(it2.x1, it2.y1))) {
            return new juce::Point<float>(it2.x1, it2.y1);
        }
    }

}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

bool MainComponent::keyPressed(const KeyPress& k) {
    bool result = false;
    /*if (k.getTextCharacter() == 'w') {//87
        car.accelerate(0.1f);
        result = true;
    } else if (k.getTextCharacter() == 's') {//83
        car.accelerate(-0.1f);
        result = true;
    }*/
    /*if (k.getTextCharacter() == 'd') {//68
        car.rotate(0.1f);
        return true;
    }
    if (k.getTextCharacter() == 'a') {//65
        car.rotate(-0.1f);
        return true;
    }*/
    
    return result;
}

bool MainComponent::keyStateChanged(bool isKeyDown) {
    return false;
}

bool MainComponent::processKeysState() {
    bool result = false;
    if (KeyPress::isKeyCurrentlyDown(87)) {//W
        userCar.accelerate(0.1f);
        result = true;
    }
    else if (KeyPress::isKeyCurrentlyDown(83)) {//S
        userCar.decelerate(0.1f);
        result = true;
    }
    if (KeyPress::isKeyCurrentlyDown(68)) {//A
        userCar.steer(0.02f);
        result = true;
    }
    else if (KeyPress::isKeyCurrentlyDown(65)) {//D
        userCar.steer(-0.02f);
        result = true;
    }

    if (KeyPress::isKeyCurrentlyDown(juce::KeyPress::spaceKey)) {
        userCar.addImpulse(juce::Point<float>(-1.33, 0));
    }

    if (KeyPress::isKeyCurrentlyDown(juce::KeyPress::homeKey)) {
        userCar.setPosition(120, 120);
    }
    return result;
}