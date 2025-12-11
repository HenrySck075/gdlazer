#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/BuildOwner.hpp"
#include "gdlazer/caffeine/foundation/BuildScope.hpp"

std::shared_ptr<Element> RootWidget::createElement() {
  return std::make_shared<RootElement>(
    std::static_pointer_cast<RootWidget>(shared_from_this())
  );
}

void RootElement::assignOwner(const std::shared_ptr<BuildOwner>& owner) {
  m_owner = owner;
  m_parentBuildScope = std::make_shared<BuildScope>();
}

void RootElement::visitChildren(ElementVisitor visitor) {
  if (m_child) {
    visitor(m_child);
  }
}

void RootElement::mount(Element* parent, void* newSlot) {
  // Root element has no parent
  if (parent != nullptr) {
    throw std::runtime_error("RootElement cannot be mounted under another element");
  }

  Element::mount(nullptr, newSlot);
  rebuild();
  Element::performRebuild(); // Clear dirty flag
}

void RootElement::update(Widget* newWidget) {
  assert(newWidget != nullptr);
  m_widget = std::shared_ptr<Widget>(newWidget);
  rebuild();
}

void RootElement::performRebuild() {
  Element::performRebuild();
}

void RootElement::rebuild() {
  try {
    auto rootWidget = std::static_pointer_cast<RootWidget>(m_widget);
    Widget* childWidget = const_cast<Widget*>(rootWidget->getChild());
    m_child = updateChild(m_child, childWidget, nullptr);
  } catch (const std::exception& e) {
    // Log error but don't crash - render tree is unavailable anyway
    m_child = nullptr;
  }
}
