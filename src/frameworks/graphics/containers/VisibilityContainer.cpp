#include "VisibilityContainer.hpp"

GDF_NS_START
bool VisibilityContainer::init() {
  if (!Container::init()) return false;
  m_shown.addCallback([this](bool, bool value) {
    if (value) onOpen();
    else onClose(); 
  });
  return true;
}

void VisibilityContainer::show() {
  m_shown = true;
};
void VisibilityContainer::hide() {
  m_shown = false;
};
void VisibilityContainer::toggleVisibility() {
  m_shown = !m_shown;
};
bool gdlazer::frameworks::VisibilityContainer::doDispatchEvent(
    Event *event, std::type_index type) {
  if (type != typeid(MouseEvent) || m_shown)
    return Container::doDispatchEvent(event, type);
  return true;
}

GDF_NS_END