#include "FillFlowContainer.hpp"

bool FillFlowContainer::init(FillDirection dir) {
    Container::init();
    direction = dir;
    addListener("nodeLayoutUpdate", [this](NodeEvent* e){
        auto ee=static_cast<NodeUIEvent*>(e);
        if (ee->type==NodeUIEventType::Position||ee->type==NodeUIEventType::All) {
            updateChildPosition();
        }
    });
    return true;
}

void FillFlowContainer::updateChildPosition() {
    // child nodes after the update
};
