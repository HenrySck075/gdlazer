#pragma once

#include "VisibilityContainer.hpp"
// yeah thats it
// @note Cascade opacity has been disabled
class OverlayContainer : public VisibilityContainer {
protected:
  Container* main;
public:
  bool init();

  void onClick(MouseEvent* e) override;
  virtual void onDismiss() {hide();}
};

class OverlayEvent : public NodeEvent {
public:
  enum class Type {
    Popout,
    Popin
  };
private:
  OverlayContainer* overlay;
  OverlayEvent::Type eventType;
public:
  OverlayEvent(OverlayContainer* o, Type type) 
  : NodeEvent("overlayEvent"), overlay(o), eventType(type) {
  }

  OverlayContainer* getOverlay() {return overlay;}
  Type getEventType() {return eventType;}
};


