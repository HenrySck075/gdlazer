#include "ButtonSystem.hpp"
#include "ButtonArea.hpp"
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
                "osu-logo-downbeat.wav"_spr, 
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
    buttonsTopLevel.push_back(
        postButtonSetup(
            MainMenuButton::create(
                "Browse", 
                "button-default-select.wav"_spr, 
                icons.Beatmap, 
                Color4(165, 204, 0, 255), 
                [](CCNode* j) {log::debug("g");}
            )
        )
    );
    buttonsTopLevel.push_back(
        postButtonSetup(
            MainMenuButton::create(
                "Exit", 
                "", 
                icons.CrossCircle, 
                Color4(238, 51, 153, 255), 
                [this](CCNode* j) {
                    this->m_menuLayerPtr = static_cast<MenuLayer*>(MenuLayer::create());
                    m_menuLayerPtr->onQuit(this);
                }
            )
        )
    );
    
    float w = CCDirector::sharedDirector()->getWinSize().width;
    this->setContentSize(CCSize(w,BUTTON_AREA_HEIGHT));
    this->logo->setZOrder(1);
    auto an = ccp(w/2-WEDGE_WIDTH*4,BUTTON_AREA_HEIGHT/2);
    logo->setPosition(an);
    logo->setScale(0.4);
    this->setPositionX(0);
    this->addChild(logo);
    auto area = ButtonArea::create(an);
    this->addChild(area);
    area->constructButtons(buttonsTopLevel);
    this->ignoreAnchorPointForPosition(false);
    return true;
}

MainMenuButton* ButtonSystem::postButtonSetup(MainMenuButton* btn) {
    btn->setHoverEnabled(false);
    return btn;
    //btn->addListener(reactive_selector(ButtonSystem::resize));
};
void ButtonSystem::resize(ReactiveNode* idc, Property prop) {
    if (prop == Property::Size) idc->getParent()->updateLayout();
}