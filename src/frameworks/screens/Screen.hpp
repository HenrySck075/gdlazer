#pragma once
#include "../graphics/containers/Container.hpp"
#include <string>

GDF_NS_START

class Screen;

struct ScreenTransitionEvent {
  Screen* last;
  Screen* next;
};

/// Also scene but handled completely differently since I didn't bother to figure out
/// what to do with TransitionScene
///
/// btw transition functions are `onScreenEnter` and `onScreenExit`
class Screen : public Container {
public:
  static Screen* create() {
    $createClass(Screen, init);
  };
  bool init() override {
    if (!Container::init()) return false;
    setOpacity(255);
    setContentSize({100,100},Unit::Percent);
    return true;
  }
  virtual void onScreenEnter(ScreenTransitionEvent event) {};
  virtual void onScreenExit(ScreenTransitionEvent event) {};
  void setTitle(std::string title) {m_title = title;}
  std::string getTitle() {return m_title;}
private:
  std::string m_title;
};

GDF_NS_END
