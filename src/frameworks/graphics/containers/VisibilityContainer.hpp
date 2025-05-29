#pragma once

#include "Container.hpp"
#include "../../bindables/Bindable.hpp"

GDF_NS_START
/// abstractions
///
/// to not increase file size i will convert all lambdas into class members instead
class VisibilityContainer : public Container {
protected:
  friend class Game;
  Bindable<bool> m_shown = false;
public: 
  bool init();
  bool isOpen() {return m_shown;}
  virtual void show();
  virtual void hide();
  void toggleVisibility();

  /// VisibilityContainer will explicitly cancels mouse events if its considered "not shown"
  bool doDispatchEvent(Event *event, std::type_index type) override;

  virtual void onOpen() {};
  virtual void onClose() {};
  
};
GDF_NS_END