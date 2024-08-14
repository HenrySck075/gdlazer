#ifndef __osu_ui_deco_triangles__
#define __osu_ui_deco_triangles__

#include "Geode/Geode.hpp"
using namespace geode::prelude;
#include "../../../../helpers/CustomActions.hpp"

class Triangles : public CCLayerRGBA {
private:
    ccColor3B m_color;

    bool init(CCSize size, int quantity, ccColor3B color);

    // TODO: tweak this because cocos2d space is not based on real window size
    float triangle_size = 100;
    float base_velocity = 50;

    CCSprite* makeTriangle();
    void spawnTriangle();
    // mf crashes when the node does not have a parent :laugh:
    void assignAction(CCNode* node, float startTime);
public:
    static Triangles* create(CCSize size, int quantity, ccColor3B baseColor);
};

#endif
