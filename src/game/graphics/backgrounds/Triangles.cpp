#include "Triangles.hpp"
#include "../../../utils.hpp"

Triangles* Triangles::create(int quantity, ccColor3B baseColor) {
    auto ret = new Triangles();
    if (ret && ret->init(quantity, baseColor)) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool Triangles::init(int quantity, ccColor3B color) {
    m_color = color;
    int triangles = quantity;

    this->setCascadeOpacityEnabled(true);
    for (int idc = 0; idc < triangles; idc++) {
        auto tri = makeTriangle();
        this->addChild(tri);
        assignAction(tri, randomFloat()*13+5);
    }
    return true;
}
void Triangles::spawnTriangle() {
    auto tri = makeTriangle();
    this->addChild(tri);
    assignAction(tri, 0);
}
void Triangles::assignAction(CCNode* node, float startTime) {
    auto space = this->getContentSize();

    auto dur = randomFloat() * 15 + 10;
    if (startTime > dur) {
        startTime = dur - 3;
    }
    float offset = randomFloat()*2;
    auto moveToAction = CCMoveTo::create(dur, CCPoint{ node->getPositionX() + (offset*randomBool()), space.height + node->getContentHeight() * node->getScale()});

#define no(...) node->runAction(CCSequence::create( \
        __VA_ARGS__, \
        CCCallFunc::create(this, callfunc_selector(Triangles::spawnTriangle)), \
        CCRemoveSelf::create(), \
        nullptr \
    ))
    no(CCActionSkip::create(startTime, moveToAction));
 

}
CCSprite* Triangles::makeTriangle() {
    auto s = CCSprite::createWithSpriteFrameName("tri_fill.png"_spr);
    int shiftValue = randomFloat()*10;
    s->setColor(ccc3(m_color.r+shiftValue, m_color.g + shiftValue, m_color.b + shiftValue));
    
    auto size = randomFloat() * 2+0.5;
    if (size == 0) { size = 1; }
    s->setScale(size);

    auto space = this->getContentSize();

    s->setPosition(CCPoint{ randomFloat() * space.width, 0});
    s->setAnchorPoint(CCPoint{ 0.5, 1 });
    
    return s;
}
