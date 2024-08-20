#pragma once

#include <Geode/Geode.hpp>
//#include "ButtonSystem.hpp"
//#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../../framework/screens/Screen.hpp"
class OsuGame;
#include "../../OsuGame.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer*;

class MainMenu : public Screen {
public:
    float FADE_IN_DURATION = 300;

    float FADE_OUT_DURATION = 400;

    bool hideOverlaysOnEnter = false;//buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

    static MainMenu* create(bool flash) {
        create_class(MainMenu, init, flash);
    }
    bool init(bool flash);

    // open the menu
    void onLogoClickIdle();
private:
    void debugReturn(CCObject* t) {
        CCDirector::sharedDirector()->popSceneWithTransition(0.5,PopTransition::kPopTransitionFade);
        while (OsuGame::get()->popScreen()) {/*meow*/};
    }
    void onExiting(ScreenTransitionEvent e) override {
        setVisible(false);
        if (e.Destination != nullptr) removeFromParent();
    }
    void testDispatch(CCObject* e) {
        auto m = new NodeEvent("googoo gaga");
        m->setDispatchingFlow(DispatchingFlow::Down);
        dispatchEvent(m);
    }
    //BeatDetector* detector;

    CCLayer* songSelect;

    MenuSideFlashes sideFlashes;

protected:
    int idkTho = 0;
  //ButtonSystem buttons;
};
