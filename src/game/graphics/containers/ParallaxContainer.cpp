#include "ParallaxContainer.hpp"

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
    Container::init();
    this->m_parallaxAmount = parallaxAmount;
    this->director = CCDirector::sharedDirector();
    this->setAnchorPoint(ccp(0.5, 0.5));
    //this->setPosition(director->getWinSize() / 2);
    this->setAnchor(Anchor::Center);
    this->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    if (scale) this->setScale(1 + abs(parallaxAmount));
    // cancel the event

    // this is done to prevent unnecessary event cascading but 
    // i will add some checks in the future

    // this is added later so it will be called after the layout update function finishes
    //addListener("nodeLayoutUpdate", [](NodeEvent* e){e->stopPropagation();});
    /*
    m_listener = this->template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
        // iykyk
        return false;
    }, false);
    */
    //updateParallax(director->convertToGL(director->getOpenGLView()->getMousePosition()));
    return true;
}

void ParallaxContainer::updateParallax(const CCPoint& cursorPos) {
    auto ws = director->getWinSize()/2;
    auto dist = (cursorPos - ws) * m_parallaxAmount;
    dist.y = -dist.y;
    this->setPosition(dist);
}
