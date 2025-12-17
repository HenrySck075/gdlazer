#pragma once

#include "gdlazer/caffeine/widgets/framework/Element.hpp"
namespace caffeine {

class StatefulElement;
class StatefulWidget;
class Widget;

class State {
  friend class StatefulElement;
protected:
  std::shared_ptr<StatefulElement> m_element;
  std::shared_ptr<StatefulWidget> m_widget;
public:
  virtual void initState() {}
  virtual void didUpdateWidget(std::shared_ptr<StatefulWidget> oldWidget) {}
  virtual void dispose() {}
  virtual Widget* build(std::shared_ptr<BuildContext> context) = 0;
  void setState(std::function<void()> fn);

  std::shared_ptr<BuildContext> getContext();

  virtual ~State() = default;
};
}
