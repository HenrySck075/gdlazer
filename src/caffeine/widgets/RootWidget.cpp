#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/widgets/framework/Element.hpp"
#include "gdlazer/caffeine/widgets/binding/BuildOwner.hpp"
#include "gdlazer/caffeine/widgets/binding/BuildScope.hpp"

namespace caffeine {
RefNauseam<Element> RootWidget::createElement() {
  return new RootElement(this);
}

void RootElement::assignOwner(const std::shared_ptr<BuildOwner>& owner) {
  m_owner = owner;
  m_parentBuildScope = std::shared_ptr<BuildScope>(new BuildScope());
}

void RootElement::visitChildren(ElementVisitor visitor) {
  if (m_child) {
    visitor(m_child);
  }
}

void RootElement::mount(RefNauseam<Element> parent, void* newSlot) {
  // Root element has no parent
  if (parent != nullptr) {
    throw std::runtime_error("RootElement cannot be mounted under another element");
  }

  Element::mount(nullptr, newSlot);
  rebuild();
}

void RootElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = newWidget;
  rebuild();
}

void RootElement::performRebuild() { 
  auto rootWidget = dynamic_cast<RootWidget*>(m_widget.get());
  Widget* childWidget = const_cast<Widget*>(rootWidget->getChild());
  m_child = updateChild(m_child, childWidget, nullptr);
  Element::performRebuild();
}

}
