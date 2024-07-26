#include "ButtonSystem.hpp"
#include "../../graphics/OsuIcon.hpp"

void ButtonSystem::setOsuLogo(OsuLogo* logo) {
        this->logo = logo;
        /*
        if (this->logo != nullptr)
        {
            this->logo.Action = onOsuLogo;

            // osuLogo.SizeForFlow relies on loading to be complete.
            buttonArea.Flow.Position = ccp(WEDGE_WIDTH * 2 - (BUTTON_WIDTH + this->logo.SizeForFlow / 4), 0);

            updateLogoState();
        }
        else
        {
            // We should stop tracking as the facade is now out of scope.
          
          logoTrackingContainer.StopTracking();
        }
        */
    }

bool ButtonSystem::init() {
    auto icons = OsuIcon();
    // toplevel
    buttonsTopLevel.push_back(MainMenuButton::create("Play", "button-play-select.wav"_spr, icons.Play, Color4(102, 68, 204, 255), [](CCNode* j) {}));
    buttonsTopLevel.push_back(MainMenuButton::create("Edit", "button-play-select.wav"_spr, icons.EditCircle, Color4(238, 170, 0, 255), [](CCNode* j) {}));

    
    return true;
}

#define reactive_selector(f) (ReactiveNode::CCReactiveCallback)(&f)

MainMenuButton* ButtonSystem::postButtonSetup(MainMenuButton* btn, bool leftAnchor) {
    btn->setAnchorPoint(ccp(leftAnchor ? 0 : 1, 0.5));
    btn->addListener(reactive_selector(ButtonSystem::resize));
};
void ButtonSystem::resize(ReactiveNode* idc) {
    idc->getParent()->updateLayout();
}