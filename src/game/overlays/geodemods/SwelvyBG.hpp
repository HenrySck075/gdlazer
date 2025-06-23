#pragma once

#include <Geode/ui/General.hpp>

using namespace geode::prelude;

class SwelvyBG : public CCNode {
private:
    float m_speed;

protected:
    bool init();

    void updateSpritePosition(float dt);

    CCArrayExt<CCSprite*> m_swelvyLayers;

public:
    static SwelvyBG* create();

    void setContentSize(const CCSize &size) override;

};