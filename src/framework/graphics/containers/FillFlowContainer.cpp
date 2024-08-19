#include "FillFlowContainer.hpp"

bool FillFlowContainer::init(FillDirection dir) {
    Container::init();
    direction = dir;
    addListener("nodeLayoutUpdate", [this](NodeEvent* e){
    });
    return true;
}

void FillFlowContainer::updateChildPosition() {
    // child nodes after the update will reset their position back so

    float x = 0;
    float y = 0;
    float yh = 0;

    bool dirFull = direction == FillDirection::Full;
    bool dirVerti = direction == FillDirection::Vertical;
    for (auto* c : CCArrayExt<CCNode*>(getChildren())) {
        auto cw = c->CCNode::getContentSize().width;
        auto ch = c->CCNode::getContentSize().height;
        if (
            dirFull ||
            direction == FillDirection::Horizontal
        ) {
            c->CCNode::setPositionX(
                x
                +
                c->CCNode::getPositionX()
                +
                (cw * c->CCNode::getAnchorPoint().x)
            );
        }

        if (
            dirFull ||
            dirVerti
        ) {
        c->CCNode::setPositionY(
                y
                +
                c->CCNode::getPositionY()
                +
                (ch * c->CCNode::getAnchorPoint().y)
            );
        }
        x+=cw;
        if (dirFull) {
            if (yh+ch > yh) yh+=ch;
        } else {
            y+=ch;
        }
        if (x > CCNode::getPositionX() && dirFull) {
            x = 0;
            y = yh;
        }
    }
};
