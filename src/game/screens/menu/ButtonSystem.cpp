#include "ButtonSystem.hpp"
#include "../../graphics/OsuIcon.hpp"
#include "../../overlays/dialog/PopupDialog.hpp"
#include "../../overlays/toolbar/ToolbarToggleButton.hpp"
#include "../Playground.hpp"
#include "ButtonConstants.hpp"

float ButtonSystem::WEDGE_WIDTH = 20;

class SongSelect;
#include "../select/SongSelect.hpp"

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
    Container::init();
    setOsuLogo(logo);
    this->m_menuLayerPtr = GameManager::sharedState()->m_menuLayer;
    this->m_creatorLayerPtr = CreatorLayer::create();

    //float w = CCDirector::sharedDirector()->getWinSize().width;
    auto an = ccp(-WEDGE_WIDTH*4,BUTTON_AREA_HEIGHT/2);
    area = ButtonArea::create(an);
    area->setID("buttonarea");
    this->addChild(area);

    // because cocos2d-x does not allows a node to be in multiple parent (everyone knows that)
    #define backBtnCreate MainMenuButton::create( \
        "Back",                                   \
        "back-to-top.wav"_spr,                    \
        OsuIcon::PrevCircle,                      \
        Color4(51, 58, 94, 255),                  \
        [this](CCNode*j){area->pop();},           \
        {enumKeyCodes::KEY_Escape}                \
    )
    
    #define $cca(...) CCArray::create(__VA_ARGS__, nullptr)
    // daily/weekly levels
    area->constructButtons($cca(
        backBtnCreate,
        MainMenuButton::create(
            "Daily", 
            "button-default-select.wav"_spr, 
            OsuIcon::ModRelax, 
            Color4(102, 68, 204, 255),
            [this](CCNode*j){this->m_creatorLayerPtr->onDailyLevel(this);},
            {enumKeyCodes::KEY_D}
        ),
        MainMenuButton::create(
            "Weekly",
            "button-default-select.wav"_spr, 
            OsuIcon::ModSuddenDeath,
            Color4(94, 63, 186, 255), 
            [this](CCNode*j){this->m_creatorLayerPtr->onWeeklyLevel(this);},
            {enumKeyCodes::KEY_W}
        )
    ), "challenges");
    area->constructButtons($cca(
        backBtnCreate,
        MainMenuButton::create(
            "Main levels", 
            "button-default-select.wav"_spr, 
            OsuIcon::Player, 
            Color4(102, 68, 204, 255),
            [this](CCNode*j){
                //this->m_menuLayerPtr->onPlay(this->m_menuLayerPtr);
                OsuGame::get()->pushScreen(SongSelect::create())->setZOrder(-7);
            },
            {enumKeyCodes::KEY_M}
        ),
        MainMenuButton::create(
            "Saved levels", 
            "button-default-select.wav"_spr, 
            OsuIcon::Online, 
            Color4(94, 63, 186, 255), 
            [this](CCNode*j){this->m_creatorLayerPtr->onSavedLevels(this);},
            {enumKeyCodes::KEY_P}
        )
    ),"play_local");
    // play    
    area->constructButtons($cca(
        backBtnCreate,
        MainMenuButton::create(
            "Local", 
            "button-play-select.wav"_spr, 
            OsuIcon::Player, 
            Color4(102, 68, 204, 255),
            [this](CCNode*j){area->show("play_local");},
            {enumKeyCodes::KEY_P}
        ),
        MainMenuButton::create(
            "Multiplayer", 
            "button-default-select.wav"_spr, 
            OsuIcon::Online, 
            Color4(94, 63, 186, 255), 
            [](CCNode*j){PopupDialog::createSimpleDialog(
                "Not implemented yet!", "mrow", 
                ":sphere_thumbsup:", "alr", 
                [](CCNode*wfhuio){CCApplication::sharedApplication()->openURL("https://www.pixiv.net/en/artworks/112989297");}
            )->show();},
            {enumKeyCodes::KEY_M}
        ),
        MainMenuButton::create(
            "Playlists", // this is lists
            "button-default-select.wav"_spr, 
            OsuIcon::Tournament, 
            Color4(94, 63, 186, 255), 
            [](CCNode*j){PopupDialog::createSimpleDialog(
                "hey guys robtopgames here", "geometry dash pdb in bio", 
                "yipee", "nuh uh", 
                [](CCNode*wfhuio){CCApplication::sharedApplication()->openURL("https://www.pixiv.net/en/artworks/120777141");}
            )->show();},
            {enumKeyCodes::KEY_L}
        ),
        MainMenuButton::create(
            "Challenges", // daily/weekly levels
            "button-play-select.wav"_spr, 
            OsuIcon::DailyChallenge,
            Color4(94, 63, 186, 255), 
            [this](CCNode*j) {area->show("challenges");},
            {enumKeyCodes::KEY_C}
        )
    ), "play");

    // edit
    area->constructButtons($cca(
        backBtnCreate,
        MainMenuButton::create(
            "New", 
            "button-default-select.wav"_spr, 
            OsuIcon::ChangelogA, 
            Color4(238, 170, 0, 255), 
            [](CCNode*fowehui) {
                LevelBrowserLayer::create(GJSearchObject::create(SearchType::MyLevels))->onNew(fowehui);                
            },
            {enumKeyCodes::KEY_B, enumKeyCodes::KEY_E}
        ),
        MainMenuButton::create(
            "Created levels", 
            "button-default-select.wav"_spr, 
            OsuIcon::Collections, 
            Color4(220, 160, 0, 255), 
            [this](CCNode*j){this->m_creatorLayerPtr->onMyLevels(this);},
            {enumKeyCodes::KEY_S}
        )
    ), "edit");
    
    // debug
    area->constructButtons($cca(
        backBtnCreate,
        MainMenuButton::create(
            "Open toolbar", 
            "button-default-select.wav"_spr, 
            OsuIcon::Beatmap, 
            Color4(165, 204, 0, 255), 
            [](CCNode* j) {
                OsuGame::get()->showToolbar();
                //->setUserObject("osugame.show-toolbar"_spr, CCBool::create(true));
            }
        ),
        MainMenuButton::create(
            "Close toolbar", 
            "button-default-select.wav"_spr, 
            OsuIcon::Beatmap, 
            Color4(154, 198, 0, 255), 
            [](CCNode* j) {
                OsuGame::get()->hideToolbar();
                //CCDirector::sharedDirector()->getRunningScene()->setUserObject("osugame.show-toolbar"_spr, CCBool::create(false));
            }
        ),
        MainMenuButton::create(
            "Playground", 
            "button-default-select.wav"_spr, 
            OsuIcon::Debug, 
            Color4(154, 198, 0, 255), 
            [](CCNode* j) {
                OsuGame::get()->pushScreen(Playground::create());
                //CCDirector::sharedDirector()->getRunningScene()->setUserObject("osugame.show-toolbar"_spr, CCBool::create(false));
            }
        )
    ), "debug");

    // toplevel
    area->constructButtons($cca(
        MainMenuButton::create(
            "Settings", 
            "osu-logo-downbeat.wav"_spr, 
            OsuIcon::Settings, 
            Color4(85, 85, 85, 255), 
            [this](CCNode*j){
                static_cast<ToolbarToggleButton*>(OsuGame::get()->getChildByIDRecursive("settings"))->select();
            }
        ),
        MainMenuButton::create(
            "Play", 
            "button-play-select.wav"_spr, 
            OsuIcon::Logo, 
            Color4(102, 68, 204, 255), 
            [this](CCNode* j) {area->show("play");},
            {enumKeyCodes::KEY_P, enumKeyCodes::KEY_M, enumKeyCodes::KEY_L}
        ),
        MainMenuButton::create(
            "Edit", 
            "button-play-select.wav"_spr, 
            OsuIcon::EditCircle, 
            Color4(238, 170, 0, 255), 
            [this](CCNode* j) {area->show("edit");},
            {enumKeyCodes::KEY_E}
        ),
        MainMenuButton::create(
            "Browse", 
            "button-play-select.wav"_spr, 
            OsuIcon::Beatmap, 
            Color4(165, 204, 0, 255), 
            [this](CCNode* j) {area->show("debug");},
            {enumKeyCodes::KEY_B, enumKeyCodes::KEY_D}
        ),
        MainMenuButton::create(
            "Exit", 
            "", 
            OsuIcon::CrossCircle, 
            Color4(238, 51, 153, 255), 
            [this](CCNode* j) {
                PopupDialog::createSimpleDialog(
                    "Are you sure you want to exit GD?",
                    "Last chance to turn back",

                    "lemme out i need to take a bath",
                    "nvm the 20-20-20 rule sucks", [this](CCNode* s) {m_menuLayerPtr->endGame(); }
                )->show();
            },
            {enumKeyCodes::KEY_Q  }
        )
    ), "toplevel");

    area->show("toplevel");
    
    this->setAnchor(Anchor::Center);
    this->setContentSizeWithUnit(CCSize(100,BUTTON_AREA_HEIGHT),Unit::Percent,Unit::OpenGL);
    logo->setZOrder(1);
    logo->setPosition(ccp(an.x,0));
    logo->setScale(0.4);
    logo->setAnchor(Anchor::Center);
    logo->setCallback([this](CCNode* self){
        auto cur = area->getCurrent();
        if (cur.has_value()) 
            static_cast<MainMenuButton*>(
                getChildByIDRecursive("buttonarea_"+cur.value())->getChildByTag(2)->getChildren()->lastObject()
            )->click();
    });
    
    //this->setPositionX(0);
    //auto me = CCMenu::createWithItem(logo);
    //me->setPosition(ccp(0,0));
    this->addChild(logo);
    setAnchorPoint(ccp(0.5,0.5));

    return true;
}

/*
void ButtonSystem::resize(ReactiveNode* idc, Property prop) {
    if (prop == Property::Size) idc->getParent()->updateLayout();
}
*/
