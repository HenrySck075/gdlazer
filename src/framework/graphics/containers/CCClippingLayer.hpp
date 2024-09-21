#pragma once
#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

// j
class CCClippingLayer : public CCLayerColor {
    CCNode* stencil;
    bool inverted = false;
public:
    void visit() override;
    bool init(CCNode* stencil_) {
        if (!CCLayerColor::initWithColor({255,255,255,0})) return false;
        stencil = stencil_;
        stencil->retain();
        return true;
    }
    ~CCClippingLayer() {
        stencil->release();
    }
    //static CCClippingLayer* create(float radius) {}
};