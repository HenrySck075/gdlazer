#pragma once

#include <gdlazer/caffeine/foundation/utils/Ref.hpp>
#include "gdlazer/caffeine/widgets/framework/Element.hpp"
namespace caffeine {

class StatefulElement;
class StatefulWidget;
class Widget;

class State : public cocos2d::CCObject {
  friend class StatefulElement;
protected:
  RefNauseam<StatefulElement> m_element;
  RefNauseam<StatefulWidget> m_widget;
public:
  virtual void initState() {}
  virtual void didUpdateWidget(RefNauseam<StatefulWidget> oldWidget) {}
  virtual void dispose() {}
  virtual Widget* build(RefNauseam<BuildContext> context) = 0;
  void setState(std::function<void()> fn);

  RefNauseam<BuildContext> getContext();

  virtual ~State() = default;
};
}
