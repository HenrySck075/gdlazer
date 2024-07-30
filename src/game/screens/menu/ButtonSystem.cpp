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

    float w = CCDirector::sharedDirector()->getWinSize().width;
    auto an = ccp(w/2-WEDGE_WIDTH*4,BUTTON_AREA_HEIGHT/2);
    auto area = ButtonArea::create(an);
    this->addChild(area);

    auto backBtn = MainMenuButton::create(
        "Back", 
        "back-to_top.wav"_spr, 
        icons.PrevCircle, 
        Color4(51, 58, 94, 255),
        [area](CCNode*j){area->show(area->getCurrentIndex()-1);}
    );

    // play    
    buttonsPlay.push_back(backBtn);
    buttonsPlay.push_back(
        MainMenuButton::create(
            "Solo", 
            "button-default-select.wav"_spr, 
            icons.Player, 
            Color4(102, 68, 204, 255),
            [](CCNode*j){}
        )
    );
    buttonsPlay.push_back(
        MainMenuButton::create(
            "Multi", 
            "button-default-select.wav"_spr, 
            icons.Online, 
            Color4(94, 63, 186, 255), 
            onMultiplayer
        )
    );
    buttonsPlay.push_back(
        MainMenuButton::create(
            "Playlists", // this is lists
            "button-default-select.wav"_spr, 
            icons.Tournament, 
            Color4(94, 63, 186, 255), 
            onPlaylists
        )
    );
    buttonsPlay.push_back(
        MainMenuButton::create(
            "Challenges", // daily/weekly levels
            "button-play-select", 
            icons.DailyChallenge,
            Color4(94, 63, 186, 255), 
            [](CCNode*j) {}
        )
    );

    // edit
    buttonsEdit.push_back(backBtn);
    buttonsEdit.push_back(
        MainMenuButton::create(
            "New", 
            "button-default-select.wav"_spr, 
            icons.Beatmap, 
            Color4(238, 170, 0, 255), 
            onEditBeatmap
        )
    );
    buttonsEdit.push_back(
        MainMenuButton::create(
            "Created", 
            "button-default-select.wav"_spr, 
            icons.Collections, 
            Color4(220, 160, 0, 255), 
            onEditSkin
        )
    );
    
    // toplevel
    buttonsTopLevel.push_back(
        MainMenuButton::create(
            "Settings", 
            "osu-logo-downbeat.wav"_spr, 
            icons.Settings, 
            Color4(85, 85, 85, 255), 
            [area](CCNode* j) {}
        )
    );
    buttonsTopLevel.push_back(
        MainMenuButton::create(
            "Play", 
            "button-play-select.wav"_spr, 
            icons.Logo, 
            Color4(102, 68, 204, 255), 
            [area](CCNode* j) {area->show(1);}
        )
    );
    buttonsTopLevel.push_back(
        MainMenuButton::create(
            "Edit", 
            "button-play-select.wav"_spr, 
            icons.EditCircle, 
            Color4(238, 170, 0, 255), 
            [area](CCNode* j) {area->show(2);}
        )
    );
    buttonsTopLevel.push_back(
        MainMenuButton::create(
            "Browse", 
            "button-default-select.wav"_spr, 
            icons.Beatmap, 
            Color4(165, 204, 0, 255), 
            [](CCNode* j) {log::debug("g");}
        )
    );
    buttonsTopLevel.push_back(
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
    );
    
    this->setContentSize(CCSize(w,BUTTON_AREA_HEIGHT));
    this->logo->setZOrder(1);
    logo->setPosition(an);
    logo->setScale(0.4);
    this->setPositionX(0);
    this->addChild(logo);

    this->ignoreAnchorPointForPosition(false);
    return true;
}

void ButtonSystem::resize(ReactiveNode* idc, Property prop) {
    if (prop == Property::Size) idc->getParent()->updateLayout();
}
