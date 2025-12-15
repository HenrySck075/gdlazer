#include <gdlazer/caffeine/widgets/framework/Widget.hpp>
#include <gdlazer/caffeine/widgets/framework/ComponentElements.hpp>
#include <gdlazer/caffeine/widgets/framework/RenderObjectElements.hpp>

namespace caffeine {
bool Widget::canUpdate(Widget* newWidget, Widget* oldWidget) {
  return typeid(*newWidget) == typeid(*oldWidget) &&
         newWidget->getKey() == oldWidget->getKey();
};




std::shared_ptr<Element> StatelessWidget::createElement() { 
  return std::shared_ptr<Element>(new StatelessElement(this));
};


std::shared_ptr<Element> StatefulWidget::createElement() {
  auto elem = std::make_shared<StatefulElement>(this);
  elem->setStateElement(elem);
  return elem;
};


std::shared_ptr<Element> RenderObjectWidget::createElement() {
  return nullptr; // TODO: should create appropriate RenderObjectElement subclass
};


std::shared_ptr<Element> ProxyWidget::createElement() {
  return nullptr; // TODO: should create ProxyElement
};


std::shared_ptr<Element> SingleChildRenderObjectWidget::createElement() {
  return std::make_shared<SingleChildRenderObjectElement>(this);
};


std::shared_ptr<Element> MultiChildRenderObjectWidget::createElement() {
  return std::make_shared<MultiChildRenderObjectElement>(this);
};

}
