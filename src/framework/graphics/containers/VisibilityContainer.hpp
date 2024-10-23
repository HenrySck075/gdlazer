#pragma once

#include "Container.hpp"
#include "../../bindables/Bindable.hpp"

/// abstractions
///
/// to not increase file size i will convert all lambdas into class members instead
class VisibilityContainer : public Container {
protected:
    Bindable<bool> shown = false;
public: 
    bool init() {
        if (!Container::init()) return false;
        shown.addCallback([this](NodeEvent* e) {
            log::debug("[VisibilityContainer]: ");
            if (static_cast<ValueChangedEvent<bool>*>(e)->value) onOpen();
            else onClose(); 
        });
        return true;
    }
    bool isOpen() {return shown;}
    virtual void show();
    virtual void hide();
    void toggleVisibility();

    virtual void onOpen() {};
    virtual void onClose() {};
    
};
