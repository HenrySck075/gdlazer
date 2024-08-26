#pragma once

#include "Container.hpp"

enum class FillDirection {
    Full,
    Vertical,
    Horizontal
};

// wrapper container around geode's layout system
class FillFlowContainer : public Container {
private:
    CCSize minimumSize;
    FillDirection direction;
    void updateChildPosition();
public:
    static FillFlowContainer* create(FillDirection dir) {
        create_class(FillFlowContainer, init, dir);
    }
    void setFillDirection(FillDirection dir);
    bool init(FillDirection dir);
    void addChild(CCNode* child) {
        Container::addChild(child);
        if (auto c = typeinfo_cast<Container*>(child)) {
            //c->setAnchor(Anchor::TopLeft);
            c->setAnchorPoint(ccp(0,1));
        }
    };

    void setContentSize(CCSize const& size) override {
        setSizeConstraints(
            CCSize(
                processUnit(size.width, m_sizeUnit.first, true),
                processUnit(size.width, m_sizeUnit.second, false)
            ), maximumSize
        );
    }

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
    };
};
