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

    CCSequence* lastOffsetResetAction = CCSequence::createWithTwoActions(
        CCDelayTime::create(0.1),
        CCCallFuncL::create([this](){lastOffset = body->getRealPosition();})
    );

public:
    bool init(Container* body) {
        if (!Container::init()) return false;
        this->body = body;
        return true;
    }
    void addChild(CCNode* child) {
        if (typeinfo_cast<Container*>(child) == nullptr) return;
        auto offset = body->getRealPosition();
        body = static_cast<Container*>(child);
        body->setAnchorPoint(ccp(0,1));
        body->setAnchor(Anchor::TopLeft);
        body->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
        body->setPositionWithUnit(offset,Unit::OpenGL,Unit::OpenGL);
    }
    void onDragStart(MouseDragEvent* event) override {lastIdlePos = body->getRealPosition();};
    void onDrag(MouseDragEvent* event) override;
    void onDragEnd(MouseDragEvent* event) override;

    void moveToPos(CCPoint const& pos, bool animate = true);

    static ScrollableContainer* create(Container* body) {
        create_class(ScrollableContainer, init, body);
    }
};