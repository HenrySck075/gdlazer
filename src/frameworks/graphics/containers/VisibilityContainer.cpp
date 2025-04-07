#include "VisibilityContainer.hpp"

GDF_NS_START
bool VisibilityContainer::init() {
  if (!Container::init()) return false;
  shown.addCallback([this](bool value) {
    if (value) onOpen();
    else onClose(); 
  });
  return true;
}

void VisibilityContainer::show() {
  shown = true;
};
void VisibilityContainer::hide() {
  shown = false;
};
void VisibilityContainer::toggleVisibility() {
  shown = !shown;
};
GDF_NS_END