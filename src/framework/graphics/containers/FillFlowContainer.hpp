#pragma once

#include "Container.hpp"
#include "ContainerLayout.hpp"

enum class FillDirection {
    Full,
    Vertical,
    Horizontal
};

// axislayout rewrite
//
// dang
class FillFlowLayout : public ContainerLayout {
public:
    bool init() {return true;}
    static FillFlowLayout* create() {
        create_class(FillFlowLayout, init);
    }
    CCSize getSizeHint(CCNode* on) const override {return on->getContentSize();}
    void apply(CCNode* on);
};

// wrapper container around geode's layout system
class FillFlowContainer : public Container {
private:
    FillDirection direction;
    void updateChildPosition();
public:
    static FillFlowContainer* create(FillDirection dir) {
        create_class(FillFlowContainer, init, dir);
    }
    void addChild(CCNode* node);
    void setFillDirection(FillDirection dir);
    bool init(FillDirection dir);
    /*
    void addChild(CCNode* child) {
        if (auto c = typeinfo_cast<Container*>(child)) {
            Container::addChild(child);
            updateChildPosition();
        }
    };
    */

    void setContentSize(CCSize const& size) override {
    }

    bool dispatchEvent(NodeEvent* event) override {
        bool ret = Container::dispatchEvent(event);
        if (ret) {
            if (event->eventName() == "nodeLayoutUpdate") {
                auto e = static_cast<NodeLayoutUpdate*>(event);
                if (e->type==NodeLayoutUpdateType::Size || e->type==NodeLayoutUpdateType::All) {
                    CCLayer::updateLayout();
                }
            };
        }
        return ret;
    };
};
