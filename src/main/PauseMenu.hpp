#ifndef __osu_mainui_pause_menu__
#define __osu_mainui_pause_menu__

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class osuPauseLayer : public PauseLayer {
public:
    bool init();
    static osuPauseLayer* create();
};

#include <Geode/modify/PauseLayer.hpp>
class $modify(PauseLayer) {
    PauseLayer* create(bool idk) {
        return osuPauseLayer::create();
    }
};

#endif // !__osu_mainui_pause_menu__
