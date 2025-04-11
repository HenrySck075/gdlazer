#pragma once
#include "Container.hpp"

GDF_NS_START
/// Despite its name, this class only provides convenient setup for a [Container]
/// to behave like a "button", since [Container]s are already clickable (listenable via the click MouseEvent).
class ClickableContainer : public Container {
  std::string m_sfx;
  bool m_consumeTap;
public:
  using ButtonCallback = std::function<void(Container*)>;
  static ClickableContainer* create(std::string sfx, bool consumeTap = false);
  bool init(std::string sfx, bool consumeTap = false);
  /// For those need to add a click listener directly
  static ClickableContainer* createWithCallback(std::string sfx, ButtonCallback callback, bool consumeTap = false);
  bool initWithCallback(std::string sfx, ClickableContainer::ButtonCallback callback, bool consumeTap = false);

  /// im just setting one of them to virtual because wdym non rtti object
  virtual void click();
};

GDF_NS_END
