#include "FillFlowContainer.hpp"

bool FillFlowContainer::init(FillDirection dir) {
    Container::init();
    direction = dir;
    return true;
}

void FillFlowContainer::updateChildPosition() {
    // child nodes after the update will reset their position back so
    auto size = getRealContentSize();
    float x = 0;
    float y = 0;
    float yh = 0;

    bool dirFull = direction == FillDirection::Full;
    bool dirVerti = direction == FillDirection::Vertical;
    bool dirHori = direction == FillDirection::Horizontal;
    auto childList = getChildren();
    if (childList) {
        for (auto* c : CCArrayExt<CCNode*>(childList)) {
            auto cw = c->CCNode::getContentSize().width;
            auto ch = c->CCNode::getContentSize().height;
            if (
                dirFull ||
                dirHori 
            ) {
                c->CCNode::setPosition(ccp(
                    x
                    +
                    c->CCNode::getPosition().x
                    +
                    (cw * c->CCNode::getAnchorPoint().x)
                ,0));
            }

            if (
                dirFull ||
                dirVerti
            ) {
            c->CCNode::setPosition(ccp(0,
                    size.height 
                    -
                    (
                        y
                        +
                        c->CCNode::getPosition().y
                        +
                        (ch * c->CCNode::getAnchorPoint().y)
                    )
                ));
            }
            x+=cw;
            if (dirFull) {
                if (yh+ch > yh) yh+=ch;
            } else {
                y+=ch;
            }
            if (x > CCNode::getContentSize().width) {
                if (dirFull) {
                    x = 0;
                    y = yh;
                }
                else if (dirHori) {
                    
                }
            }
        }
    }
};


