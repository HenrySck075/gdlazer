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

std::shared_ptr<Element> ComponentElement::getAttachingRenderObjectChild() {
  return m_child;
}

void StatelessElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = std::static_pointer_cast<StatelessWidget>(std::shared_ptr<Widget>(newWidget));
  markNeedsBuild();
}

StatefulElement::StatefulElement(StatefulWidget* widget)
    : ComponentElement(widget),
      m_widget(std::shared_ptr<StatefulWidget>(widget)) {
  m_state = widget->createState();
  m_state->m_widget = m_widget;
  m_state->m_element = std::static_pointer_cast<StatefulElement>(shared_from_this());
}

void StatefulElement::mount(std::shared_ptr<Element> parent, void* slot) {
  Element::mount(parent, slot);
  m_state->initState();
}


void StatefulElement::setStateElement(std::shared_ptr<StatefulElement> self) {
  //m_state->m_element = self;
}
Widget* StatefulElement::build() {
  return m_state->build(std::shared_ptr<BuildContext>(this));
}

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


void StatefulElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  auto oldWidget = m_widget;
  m_widget = std::static_pointer_cast<StatefulWidget>(std::shared_ptr<Widget>(newWidget));
  m_state->didUpdateWidget(oldWidget);
  markNeedsBuild();
}

void ProxyElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = std::static_pointer_cast<ProxyWidget>(std::shared_ptr<Widget>(newWidget));
  markNeedsBuild();
}
} // namespace caffeine
