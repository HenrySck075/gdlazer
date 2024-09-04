#pragma once

#include <Geode/Geode.hpp>
#include "../../overlays/OverlayColorProvider.hpp"
#include "OsuOverlayContainer.hpp"
using namespace geode::prelude;

using ColorScheme = OverlayColorScheme;

class WaveContainer : public OsuOverlayContainer {
private:
    bool hiding = false;

    OverlayColorProvider* provider;

    CCPoint touchLoc = ccp(0,0);
    CCRect touchBoundary;

    CCDrawNode* wave1;
    CCDrawNode* wave2;
    CCDrawNode* wave3;
    CCDrawNode* wave4;

    CCNode* body;

    float angle1 = 13;
    float angle2 = -7;
    float angle3 = 4;
    float angle4 = -2;

    float pos1;
    float pos2;
    float pos3;
    float pos4;

    float appearDuration = 0.8f;
    float disappearDuration = 0.5f;

    CCDrawNode* createWave(float w, CCSize size, float angle, ccColor4B col);
    void onClose() override;
    void onOpen() override;

    void onMouseDown(MouseEvent* e) override;
    void onClick(MouseEvent* e) override;
public:
    bool init(ColorScheme color, CCNode* body);
    // @note RobTop addition
    bool customSetup(CCNode* body);
    static WaveContainer* create(ColorScheme color, CCNode* body);
};
