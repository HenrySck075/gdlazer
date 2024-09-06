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
    void apply(CCNode* on) {
        CCArrayExt<Container*> nodes = getNodesToPosition(on);

        auto node = static_cast<Container*>(on);

        auto constraint = node->getSizeConstraints().second;
        CCSize size = CCSize(0,0);
        for (auto c : nodes) {
            //log::debug("[FillFlowLayout]: {}",size.height);
            if (c->getSizeUnit().second == Unit::Percent && constraint.height == 0) {
                throw std::invalid_argument(fmt::format("[FillFlowContainer/Layout]: Child {} has the size dependent on the parent, but the parent does not have a maximum size constraint. Please set the maximum constraint to non-zero.", geode::format_as(c)));
            }
            c->setAnchor(Anchor::TopLeft);
            c->setPosition(ccp(0,0));
            c->setAnchorPoint(ccp(0,1));
            auto cs = c->getContentSize();
            c->CCNode::setPositionY(size.height);
            auto ns = size.height+cs.height;
            size.height = constraint.height!=0?std::min(constraint.height, ns):ns;
        }
        size.width = node->getContentSize().width;
        node->CCNode::setContentSize(size);
    };
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
    /*
    void addChild(CCNode* child) {
        if (auto c = typeinfo_cast<Container*>(child)) {
            Container::addChild(child);
            updateChildPosition();
        }
    };
    */

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
                if (e->type==NodeLayoutUpdateType::Size || e->type==NodeLayoutUpdateType::All) {
                    CCLayer::updateLayout();
                }
            };
        }
        return ret;
    };
};
