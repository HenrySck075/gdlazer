#include <gdlazer/caffeine/widgets/framework/ComponentElements.hpp>
#include <gdlazer/caffeine/widgets/framework/State.hpp>

namespace caffeine {

void ComponentElement::performRebuild() {
  auto built_widget = build();
  Element::performRebuild();
  try {
    m_child = updateChild(m_child, built_widget, m_slot);
    assert(m_child != nullptr);
  } catch (std::exception&) {
    if (m_child) m_child->deactivate();
    m_child = updateChild(nullptr, built_widget, m_slot);
  }
};

RefNauseam<Element> ComponentElement::getAttachingRenderObjectChild() {
  return m_child;
}

void StatelessElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = newWidget;
  markNeedsBuild();
}

StatefulElement::StatefulElement(StatefulWidget* widget)
    : ComponentElement(widget),
      m_widget(widget) {
  m_state = widget->createState();
  m_state->m_widget = m_widget;
  m_state->m_element = this;
}

void StatefulElement::mount(RefNauseam<Element> parent, void* slot) {
  Element::mount(parent, slot);
  m_state->initState();
  rebuild();
}

 
Widget* StatefulElement::build() {
  return m_state->build(this);
}

/*
void StatefulElement::performRebuild() {
  auto built_widget = build();
  Element::performRebuild();
  try {
    m_child = updateChild(m_child, built_widget, m_slot);
    assert(m_child != nullptr);
  } catch (std::exception&) {
    if (m_child) m_child->deactivate();
    m_child = updateChild(nullptr, built_widget, m_slot);
  }
}
*/

void StatefulElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  auto oldWidget = m_widget;
  m_widget = dynamic_cast<StatefulWidget*>(newWidget);
  m_state->didUpdateWidget(oldWidget);
  markNeedsBuild();
}

void ProxyElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = dynamic_cast<ProxyWidget*>(newWidget);
  markNeedsBuild();
}
} // namespace caffeine
