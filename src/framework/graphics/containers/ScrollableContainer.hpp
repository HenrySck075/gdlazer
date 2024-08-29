#pragma once
#include "Container.hpp"
#include "../../input/events/MouseDragEvent.hpp"
#include <Geode/cocos/include/cocos2d.h>
#include "../../../helpers/CustomActions.hpp"
#include "../../../utils.hpp"

using namespace cocos2d;

// reimplementing the wheel again
//
// only supports vertical scrolling for now
class ScrollableContainer : public Container {
private:
    Container* body;
    CCPoint lastOffset = CCPoint(0,0);
    CCPoint lastIdlePos = CCPoint(0,0);
    MouseDragEvent* lastDragEvent;
    bool m_dragging = false;
    float timer = 0;

public:
    void update(float dt) override {
        if (m_dragging) {
            timer+=dt;
            if (timer>=0.5) {
                lastOffset = lastDragEvent->startPos;
                timer = 0;
            }
        }
    }
    bool init(Container* body) {
        if (!Container::init()) return false;
        this->body = body;
        dragEnabled(true);
        addChild(body);
        scheduleUpdate();
        return true;
    }
    void addChild(CCNode* child) {
        if (typeinfo_cast<Container*>(child) == nullptr) return;
        auto offset = body->getPosition();
        removeChild(body);
        body = static_cast<Container*>(child);
        CCNode::addChild(body);
        body->setAnchorPoint(ccp(0,1));
        body->setAnchor(Anchor::TopLeft);
        //body->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
        body->setPositionWithUnit(offset,Unit::OpenGL,Unit::OpenGL);
    }
    void onDragStart(MouseDragEvent* event) override;
    void onDrag(MouseDragEvent* event) override;
    void onDragEnd(MouseDragEvent* event) override;

    void moveToPos(CCPoint const& pos, bool animate = true);

    static ScrollableContainer* create(Container* body) {
        create_class(ScrollableContainer, init, body);
    }
};