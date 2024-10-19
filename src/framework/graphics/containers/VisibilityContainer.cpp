#include "VisibilityContainer.hpp"


void VisibilityContainer::show() {
    shown = true;
};
void VisibilityContainer::hide() {
    shown = false;
};
void VisibilityContainer::toggleVisibility() {
    shown = !(bool)shown;
};