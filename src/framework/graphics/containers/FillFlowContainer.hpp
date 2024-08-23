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
    void updateChildPosition();
public:
    static FillFlowContainer* create(FillDirection dir) {
        create_class(FillFlowContainer, init, dir);
    }
    bool init(FillDirection dir);

    bool dispatchEvent(NodeEvent* event) override {
        bool ret = Container::dispatchEvent(event);
        if (ret) {
            if (event->eventName() == "nodeLayoutUpdate") {
                auto e = static_cast<NodeLayoutUpdate*>(event);
                //if (e->type==NodeLayoutUpdateType::Size) { //||e->type==NodeLayoutUpdateType::All
                    this->updateChildPosition();
                //}
            };
        }
        return ret;
    }
};
