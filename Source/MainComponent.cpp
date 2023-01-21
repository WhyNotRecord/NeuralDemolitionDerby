#include "MainComponent.h"

using namespace juce;

const float border = 20;
const int crashPauseInFrames = 10;
const int carsCount = 8;
//==============================================================================
MainComponent::MainComponent() : 
    userCar(120, 120, 100, 5.f),
    collision("")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setFramesPerSecond (60); // This sets the frequency of the update calls.
    cars.push_back(&userCar);
    userCar.setDirection(1.f);
    userCar.setId("Y");

    juce::Point<float> center(getWidth() * 0.5f, getHeight() * 0.5f);
    float radius = juce::jmin<int>(getWidth(), getHeight()) * 0.45f;
    int prev = 1, pPrev = 0;
    NDDCar* enemyCar = NULL;
    for (int i = 1; i <= 8; i++) {
        int id = prev + pPrev;

        float angle = juce::MathConstants<float>::twoPi / carsCount * i;
        juce::Point<float> pos(std::cosf(angle) * radius, std::sinf(angle) * radius);
        enemyCar = createCar(pos.x + center.x, pos.y + center.y, 100, 4.f, juce::String(id));
        enemyCar->setDirection(angle + juce::MathConstants<float>::pi);

        pPrev = prev;
        prev = id;
    }

    rDriver = NDDStraightDriver(enemyCar);//make last car alive

    setWantsKeyboardFocus(true);
}

MainComponent::~MainComponent()
{
    for (int i = 1; i < cars.size(); i++) {//кроме машины игрока
        delete cars.at(i);
    }
}

//==============================================================================
void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
    processKeysState();
    rDriver.update(getBounds());

    juce::Rectangle<int> fieldBounds = getBounds().reduced(border);
    collision = "";
    for (int i = 0; i < cars.size(); i++) {
        NDDCar* c1 = cars.at(i);
        c1->update();

        //process collisions
        for (int j = i + 1; j < cars.size(); j++) {
            NDDCar* c2 = cars.at(j);
            float dist2 = c1->getPosition().getDistanceSquaredFrom(c2->getPosition());
            if (dist2 <= c1->getMaxRadius2() + c2->getMaxRadius2()) {
                validatePossibleCollision(c1, c2);
            }
        }

        //process field borders
        juce::Point<float> cPos = c1->getPosition();
        //if (!fieldBounds.contains(cPos.x, cPos.y)) {
            if (cPos.x < fieldBounds.getX()) {
                c1->addImpulse(Point<float>(2, 0));//todo чтоб зависело от скорости въезда
            }
            else if (cPos.x > fieldBounds.getRight()) {
                c1->addImpulse(Point<float>(-2, 0));//todo чтоб зависело от скорости въезда
            }
            if (cPos.y < fieldBounds.getY()) {
                c1->addImpulse(Point<float>(0, 2));//todo чтоб зависело от скорости въезда
            }
            else if (cPos.y > fieldBounds.getBottom()) {
                c1->addImpulse(Point<float>(0, -2));//todo чтоб зависело от скорости въезда
            }
        //}
    }
    for (const auto& n : actCollisions) {
        if (n.second > 0)
            actCollisions[n.first]--;
        /*else
            actCollisions.erase(n.first);*/
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
}

NDDCar* MainComponent::createCar(float _x, float _y, int _mass, float _acceleration, juce::String id) {
    NDDCar *newCar = new NDDCar(_x, _y, _mass, _acceleration);
    newCar->setId(id);
    cars.push_back(newCar);
    return newCar;
}

void MainComponent::validatePossibleCollision(NDDCar* c1, NDDCar* c2) {
    collision = c1->getId() + "x" + c2->getId();
    juce::Point<float> imp1 = c1->getImpulse();
    juce::Point<float> imp2 = c2->getImpulse();
    juce::Point<float>* cp = getCollisionPoint(c1->getBounds(), imp1, c2->getBounds(), imp2);
    if (cp != NULL) {
        if (actCollisions[collision] == NULL) {
            actCollisions[collision] = crashPauseInFrames;
            collision += (" - " + cp->toString());
            processCollision(c1, c2, imp1, imp2, cp);
        }
        delete cp;
    }
}

void MainComponent::processCollision(NDDCar* fCar, NDDCar* sCar,
    juce::Point<float> fCarImp, juce::Point<float> sCarImp, juce::Point<float>* cp) {
    if (fCarImp.isOrigin() && sCarImp.isOrigin()) {//for cars not to stuck
        fCarImp = (*cp - fCar->getPosition()) / 2;
        sCarImp = (*cp - sCar->getPosition()) / 2;
    }
    juce::Point<float> imp = fCarImp + sCarImp;
    //float colPower = imp.getDistanceFromOrigin();
    float fCarImpPower = fCarImp.getDistanceFromOrigin();
    float sCarImpPower = sCarImp.getDistanceFromOrigin();
    float colPower = fCarImpPower + sCarImpPower;

    //fCar->brake(fCarImpPower > 1 ? fCarImpPower : colPower);
    //sCar->brake(sCarImpPower > 1 ? sCarImpPower : colPower);
    fCar->brake(colPower);
    sCar->brake(colPower);


    float help = (sCarImpPower - fCarImpPower) /** (colPower / 2)*/;
    fCar->setImpulse((fCarImpPower < 0.5f ? imp : imp / fCarImpPower) * help);
    sCar->setImpulse((sCarImpPower < 0.5f ? imp : imp / sCarImpPower) * -help);

    fCar->setStunned(crashPauseInFrames);
    sCar->setStunned(crashPauseInFrames);

    juce::Point<float> center = (fCar->getPosition() + sCar->getPosition()) / 2;

    juce::Point<float> fp = *cp - fCar->getPosition();
    fp.applyTransform(juce::AffineTransform::rotation(-fCar->getDirection()));
    float fcAng = fp.y * fp.x / (fCar->getWidth() * fCar->getLength());//fp.getDistanceFromOrigin();

    juce::Point<float> sp = *cp - sCar->getPosition();
    sp.applyTransform(juce::AffineTransform::rotation(-sCar->getDirection()));
    float scAng = sp.y * sp.x / (sCar->getWidth() * sCar->getLength());//sp.getDistanceFromOrigin();

    //fcAng и scAng зависят от точки применения силы как по знаку, так и по величине
    //float fcRotPow = fp.getDistanceFromOrigin() / fCar->getMaxRadius2();
    //float scRotPow = sp.getDistanceFromOrigin() / sCar->getMaxRadius2();

    float fcRotation = 0.2f * fcAng * /*fcRotPow */ colPower / (fCarImpPower + 1);
    float scRotation = 0.2f * -scAng * /*scRotPow */ colPower / (sCarImpPower + 1);
    //todo неверное направление вращения при ударе спереди/сзади
    juce::Logger::writeToLog(juce::String(fcRotation) + " " + juce::String(scRotation));
    fCar->startRotation(fcRotation);
    sCar->startRotation(scRotation);

    sCar->decrementHealth(fCarImpPower);
    fCar->decrementHealth(sCarImpPower);
}

juce::Point<float>* MainComponent::getCollisionPoint(juce::Path* c1Bounds, juce::Point<float> imp1,
    juce::Path* c2Bounds, juce::Point<float> imp2) {
    juce::Path::Iterator it1(*c1Bounds);
    while (it1.next()) {
        if (c2Bounds->contains(juce::Point<float>(it1.x1, it1.y1) + imp1)) {
            return new juce::Point<float>(it1.x1, it1.y1);
        }
    }
    juce::Path::Iterator it2(*c2Bounds);
    while (it2.next()) {
        if (c1Bounds->contains(juce::Point<float>(it2.x1, it2.y1) + imp2)) {
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