#include <gdlazer/caffeine/widgets/framework/Widget.hpp>
#include <gdlazer/caffeine/widgets/framework/ComponentElements.hpp>
#include <gdlazer/caffeine/widgets/framework/RenderObjectElements.hpp>

namespace caffeine {
bool Widget::canUpdate(Widget* newWidget, Widget* oldWidget) {
  return typeid(*newWidget) == typeid(*oldWidget) &&
         newWidget->getKey() == oldWidget->getKey();
};




RefNauseam<Element> StatelessWidget::createElement() { 
  return new StatelessElement(this);
};


RefNauseam<Element> StatefulWidget::createElement() {
  return new StatefulElement(this);
};

RefNauseam<Element> LeafRenderObjectWidget::createElement() {
  return new LeafRenderObjectElement(this);
};


RefNauseam<Element> SingleChildRenderObjectWidget::createElement() {
  return new SingleChildRenderObjectElement(this);
};


RefNauseam<Element> MultiChildRenderObjectWidget::createElement() {
  return new MultiChildRenderObjectElement(this);
};

} // namespace caffeine
