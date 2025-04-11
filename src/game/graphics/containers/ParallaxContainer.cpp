#include "ParallaxContainer.hpp"
#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Setting.hpp>
#include "../../OsuGame.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START
class ParallaxStateUpdated : public frameworks::Event {
public:
  bool m_enabled;
  ParallaxStateUpdated(bool e) : m_enabled(e) {};
};

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
  using namespace frameworks;
  if (!Container::init()) return false;
  m_parallaxAmount = parallaxAmount;
  m_parallax = Mod::get()->getSettingValue<bool>("parallax");
  m_director = CCDirector::sharedDirector();
  setAnchorPoint({0.5, 0.5});
  //setPosition(director->getWinSize() / 2);
  setAnchor(Anchor::Center);
  setContentSize({100,100},Unit::Percent);
  if (scale) setScale(1 + abs(parallaxAmount));
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Move) 
      updateParallax(e->m_position); 
    return true;
  });
  addListener<ParallaxStateUpdated>([this](ParallaxStateUpdated* e){
    m_parallax = e->m_enabled;
    if (!m_parallax) runAction(easingsActions::CCEaseSineOut::create(CCMoveTo::create(0.2, m_director->getWinSize()/2)));
    return false;
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
  auto ws = m_director->getWinSize()/2;
  auto dist = (cursorPos - ws) * m_parallaxAmount;
  setPosition(dist);
}
GDL_NS_END

$execute{
  listenForSettingChanges("parallax", +[](bool v){
    gdlazer::game::OsuGame::get()->dispatchEvent(new gdlazer::game::ParallaxStateUpdated(v));
  });
}
