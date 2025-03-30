#include "ClickableContainer.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "Geode/binding/FMODAudioEngine.hpp"
#include "Geode/binding/GameManager.hpp"

GDF_NS_START
ClickableContainer *
ClickableContainer::create(std::string sfx, bool consumeTap) {
  auto ret = new ClickableContainer();
  if (ret && ret->init(sfx, consumeTap)) {
    ret->autorelease();
  } else {
    delete ret;
    ret = nullptr;
  }
  return ret;
};
bool ClickableContainer::init(std::string sfx, bool consumeTap) {
  if (!Container::init()) return false;
  m_sfx = sfx;
  m_consumeTap = consumeTap;
  addListener<MouseEvent>([this](MouseEvent* e){
    FMODAudioEngine::sharedEngine()->playEffect(m_sfx, 1, 0, GameManager::sharedState()->m_sfxVolume);
    if (m_consumeTap) e->stopPropagation();
    return true;
  });
};

GDF_NS_END