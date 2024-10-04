#pragma once

#include <Geode/Geode.hpp>
//#include "ButtonSystem.hpp"
//#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "Background.hpp"
#include "ButtonSystem.hpp"
#include "../../../framework/screens/Screen.hpp"
class OsuGame;
#include "../../OsuGame.hpp"
#include "../../../utils.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer;

class MainMenu : public Screen {
    ButtonSystem* buttonSys;
    Background* bg;
    ParallaxContainer* buttonSysParallax;
    CCMoveToModifiable* logoMoveAction;
public:
    float FADE_IN_DURATION = 300;

    float FADE_OUT_DURATION = 400;

    bool hideOverlaysOnEnter = false;//buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

    static MainMenu* create() {
        create_class(MainMenu, init);
    }
    bool init();

    // open the menu
    void onLogoClickIdle();
private:
    void debugReturn(CCObject* t) {
        CCDirector::sharedDirector()->popSceneWithTransition(0.5,PopTransition::kPopTransitionFade);
        while (OsuGame::get()->popScreen()) {/*meow*/};
    }
    void onExiting(ScreenTransitionEvent e) override {
        //setVisible(false);
        if (e.Destination == nullptr) removeFromParent();
        else {
            setCascadeOpacityEnabled(true);
            runAction(CCSequence::createWithTwoActions(
                CCWaitUntil::create(bg->runAction(CCFadeOut::create(2))),
                CCFadeOut::create(1)
            ));
            buttonSys->area->hide(buttonSys->area->getCurrent().value(),true,true);
        }
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
