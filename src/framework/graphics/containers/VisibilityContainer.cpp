#include "VisibilityContainer.hpp"


bool VisibilityContainer::init() {
    if (!Container::init()) return false;
    shown.addCallback([this](NodeEvent* e) {
        log::debug("[VisibilityContainer]: ");
        if (static_cast<ValueChangedEvent<bool>*>(e)->value) onOpen();
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
    shown = !(bool)shown;
};