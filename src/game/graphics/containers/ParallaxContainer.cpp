#include "ParallaxContainer.hpp"
#include "Geode/DefaultInclude.hpp"
#include <Geode/loader/SettingEvent.hpp>
#include "../../OsuGame.hpp"

class ParallaxStateUpdated : public NodeEvent {
public:
    bool enabled;
    ParallaxStateUpdated(bool e) : NodeEvent("parallaxStateUpdate"), enabled(e) {};
};

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
    Container::init();
    this->m_parallaxAmount = parallaxAmount;
    this->director = CCDirector::sharedDirector();
    this->setAnchorPoint(ccp(0.5, 0.5));
    //this->setPosition(director->getWinSize() / 2);
    this->setAnchor(Anchor::Center);
    this->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    if (scale) this->setScale(1 + abs(parallaxAmount));
    addListener("parallaxStateUpdate", [this](NodeEvent* e){
        parallax = static_cast<ParallaxStateUpdated*>(e)->enabled;
        if (!parallax) runAction(CCEaseSineOut::create(CCMoveTo::create(0.2, director->getWinSize()/2)));
    });
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
    if (!parallax) return;
    auto ws = director->getWinSize()/2;
    auto dist = (cursorPos - ws) * m_parallaxAmount;
    dist.y = -dist.y;
    this->setPosition(dist);
}

$execute{
    listenForSettingChanges("parallax", +[](bool v){
        OsuGame::get()->dispatchEvent(new ParallaxStateUpdated(v));
    });
}
