#pragma once
#include "..\graphics\containers\Container.hpp"

GDF_NS_START

class Screen;

struct ScreenTransitionEvent {
  Screen* last;
  Screen* next;
};

/// Also scene but handled completely differently since I didn't bother to figure out
/// what to do with TransitionScene
///
/// btw said transition functions are `onScreenEnter` and `onScreenExit`
class Screen : public Container {
public:
  virtual void onScreenEnter(ScreenTransitionEvent event) {};
  virtual void onScreenExit(ScreenTransitionEvent event) {};
};

GDF_NS_END