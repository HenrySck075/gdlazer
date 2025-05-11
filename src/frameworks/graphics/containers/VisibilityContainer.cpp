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
GDF_NS_END