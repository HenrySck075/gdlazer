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

bool ButtonSystem::init(OsuLogo* logo) {
    auto icons = OsuIcon();
    setOsuLogo(logo);
    // toplevel
    buttonsTopLevel.push_back(
        postButtonSetup(
            MainMenuButton::create(
                "Settings", 
                "button-play-select.wav"_spr, 
                icons.Settings, 
                Color4(85, 85, 85, 255), 
                [](CCNode* j) {}
            )
        )
    );
    buttonsTopLevel.push_back(
        postButtonSetup(
            MainMenuButton::create(
                "Play", 
                "button-play-select.wav"_spr, 
                icons.Logo, 
                Color4(102, 68, 204, 255), 
                [](CCNode* j) {}
            )
        )
    );
    buttonsTopLevel.push_back(
        postButtonSetup(
            MainMenuButton::create(
                "Edit", 
                "button-play-select.wav"_spr, 
                icons.EditCircle, 
                Color4(238, 170, 0, 255), 
                [](CCNode* j) {}
            )
        )
    );
    // rework
    this->addChild(constructButtons(buttonsTopLevel));
    return true;
}

CCLayer* ButtonSystem::constructButtons(CCArrayExt<MainMenuButton*> buttons) {
    auto main = CCLayer::create();
    main->setContentSize(CCSize(CCDirector::sharedDirector()->getWinSize().width,BUTTON_AREA_HEIGHT));
    main->setLayout(RowLayout::create()->setAutoScale(false)->setGap(-1));
    buttons[0]->setZOrder(0);
    main->addChild(buttons[0]);

    this->logo->setZOrder(1);
    main->addChild(this->logo);

    auto otherButtons = CCLayer::create();
    otherButtons->setLayout(RowLayout::create()->setAutoScale(false)->setGap(-1)->setAxisAlignment(AxisAlignment::Start));
    otherButtons->setContentSize(CCSize(500,BUTTON_AREA_HEIGHT));
    for (int i = 1; i<buttons.size(); i++) {
        otherButtons->addChild(buttons[i]);
    }
    otherButtons->updateLayout(false);
    otherButtons->setZOrder(2);
    main->addChild(otherButtons);
    main->updateLayout(false);
    return main;
}

MainMenuButton* ButtonSystem::postButtonSetup(MainMenuButton* btn, bool leftAnchor) {
    btn->setAnchorPoint(ccp(leftAnchor ? 0 : 1, 0.5));
    return btn;
    //btn->addListener(reactive_selector(ButtonSystem::resize));
};
void ButtonSystem::resize(ReactiveNode* idc, Property prop) {
    if (prop == Property::Size) idc->getParent()->updateLayout();
}