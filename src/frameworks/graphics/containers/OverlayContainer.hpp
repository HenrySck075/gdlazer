#pragma once

#include "VisibilityContainer.hpp"
GDF_NS_START
/// yeah thats it
/// @note Cascade opacity has been disabled
/// @note If you wanted to use an infinite-looping action on show, PLEASE place it on a child node, otherwise your container will NEVER receives any events except for NodeSizeUpdated (until you stop it),
/// as the actions for the OverlayContainer is used to determine whether the enter animations finished.
class OverlayContainer : public VisibilityContainer {
public:
  bool init();

  virtual void onDismiss() {hide();}
  void show() override;
  void hide() override;

  bool doDispatchEvent(Event* event, std::type_index type);

  void update(float) override;

  void setTitle(std::string title) {m_title = title;}
  std::string getTitle() {return m_title;}

protected:
  geode::Ref<Container> m_main;
private: 
  std::string m_title;
  bool m_blockingEvents = false;
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
