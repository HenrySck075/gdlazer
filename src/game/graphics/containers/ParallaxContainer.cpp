#include "ParallaxContainer.hpp"
#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Setting.hpp>
#include "../../OsuGame.hpp"

class ParallaxStateUpdated : public NodeEvent {
public:
  bool enabled;
  ParallaxStateUpdated(bool e) : NodeEvent("parallaxStateUpdate"), enabled(e) {};
};

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
  Container::init();
  m_parallaxAmount = parallaxAmount;
  m_parallax = Mod::get()->getSettingValue<bool>("parallax");
  director = CCDirector::sharedDirector();
  setAnchorPoint({0.5, 0.5});
  //setPosition(director->getWinSize() / 2);
  setAnchor(Anchor::Center);
  setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
  if (scale) setScale(1 + abs(parallaxAmount));
  addListener("parallaxStateUpdate", [this](NodeEvent* e){
    m_parallax = static_cast<ParallaxStateUpdated*>(e)->enabled;
    if (!m_parallax) runAction(CCEaseSineOut::create(CCMoveTo::create(0.2, director->getWinSize()/2)));
  });
  // cancel the event

  // this is done to prevent unnecessary event cascading but 
  // i will add some checks in the future

  // this is added later so it will be called after the layout update function finishes
  //addListener("nodeLayoutUpdate", [](NodeEvent* e){e->stopPropagation();});
  /*
  m_listener = template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
    // iykyk
    return false;
  }, false);
  */
#ifndef GEODE_IS_ANDROID
  //updateParallax(director->getOpenGLView()->getMousePosition());
#endif
  return true;
}

void ParallaxContainer::updateParallax(const CCPoint& cursorPos) {
  if (!m_parallax) return;
  auto ws = director->getWinSize()/2;
  auto dist = (cursorPos - ws) * m_parallaxAmount;
  dist.y = -dist.y;
  setPosition(dist);
}

$execute{
  listenForSettingChanges("parallax", +[](bool v){
    OsuGame::get()->dispatchEvent(new ParallaxStateUpdated(v));
  });
}
