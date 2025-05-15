#pragma once

#include "VisibilityContainer.hpp"
GDF_NS_START
// yeah thats it
// @note Cascade opacity has been disabled
class OverlayContainer : public VisibilityContainer {
protected:
  geode::Ref<Container> m_main;
public:
  bool init();

  virtual void onDismiss() {hide();}
};

class OverlayEvent : public Event {
public:
  enum class Type {
    Popout,
    Popin
  };
private:
  geode::Ref<OverlayContainer> overlay;
  OverlayEvent::Type eventType;
public:
  OverlayEvent(OverlayContainer* o, Type type) 
  : overlay(o), eventType(type) {
  }

  OverlayContainer* getOverlay() {return overlay;}
  Type getEventType() {return eventType;}
};
GDF_NS_END
