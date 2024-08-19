#pragma once

#include "Container.hpp"

enum class FillDirection {
    Full,
    Vertical,
    Horizontal
};

class FillFlowContainer : public Container {
private:
    FillDirection direction;
public:
    static FillFlowContainer* create(FillDirection dir) {
        create_class(FillFlowContainer, init, dir);
    }
    bool init(FillDirection dir);
    void updateChildPosition();

    bool dispatchEvent(NodeEvent* event) override {
        Container::dispatchEvent(event);
        if (event->eventName() == "nodeLayoutUpdate") {
            auto e=static_cast<NodeUIEvent*>(event);
            if (e->type==NodeUIEventType::Position||e->type==NodeUIEventType::All) {
                updateChildPosition();
            }
        }
    }
};
