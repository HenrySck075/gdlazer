#include "Widget.hpp"
#include "foundation/ComponentElements.hpp"
bool Widget::canUpdate(shared_ptr_ctor<Widget> newWidget,
                       shared_ptr_ctor<Widget> oldWidget) {
  return typeid(newWidget.get()) == typeid(oldWidget.get()) &&
         newWidget->key == oldWidget->key;
};




std::shared_ptr<Element> StatelessWidget::createElement() { 
  return StatelessElement(this);
};
