#include "ClickableContainer.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "Geode/binding/FMODAudioEngine.hpp"
#include "Geode/binding/GameManager.hpp"
#include "../../../utils.hpp"

GDF_NS_START
ClickableContainer* ClickableContainer::create(std::string sfx, bool consumeTap) {
  $create_class(ClickableContainer, init, sfx, consumeTap);
};
ClickableContainer* createWithCallback(std::string sfx, ButtonCallback callback, bool consumeTap) {
  $create_class(ClickableContainer, initWithCallback, sfx, callback, consumeTap);
};

bool ClickableContainer::initWithCallback(std::string sfx, ButtonCallback callback, bool consumeTap) {
  if (!ClickableContainer::init(sfx, consumeTap)) return false;
  m_callback = callback;
  m_callbackSet = true;
  return true;
}

bool ClickableContainer::init(std::string sfx, bool consumeTap) {
  if (!Container::init()) return false;
  m_sfx = sfx;
  m_consumeTap = consumeTap;
  setTouchEnabled(true);
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click) {
      FMODAudioEngine::sharedEngine()->playEffect(m_sfx/*, 1, 0, GameManager::sharedState()->m_sfxVolume*/);
      if (m_callbackSet) m_callback(this);
      //if (m_consumeTap) e->stopPropagation();
    }
    return true;
  });
  return true;
};
void ClickableContainer::click() {
  auto box = boundingBoxFromContentSize(this);
  cocos2d::CCPoint p{box.getMidX(), box.getMidY()};
  dispatchEvent(new MouseEvent(MouseEventType::Click, p, true));
};
GDF_NS_END
