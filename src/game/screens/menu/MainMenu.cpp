#include "MainMenu.hpp"
#include "MainMenuButton.hpp"
#include "ButtonSystem.hpp"
#include "OsuLogo.hpp"
#include "../../graphics/OsuIcon.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"

bool MainMenu::init(bool flash) {
    Container::init();
    auto logo = OsuLogo::create();
    //logo->setPosition(CCDirector::sharedDirector()->getWinSize() / 2);
    //logo->setCallback([this](CCNode* j) {onLogoClickIdle(); });
    //auto g = MainMenuButton::create("g","button-play-select.wav"_spr, OsuIcon().Logo, Color4Defined::Green, [this](CCNode* idk) {});
    //detector = BeatDetector::Instance();
    //detector->LoadSongFromSystem();
    int bgIndex = (int)(randomFloat() * 7) + 1;
    std::string the;
    // this is not yandere behaviour, simply because of _spr
    switch (bgIndex) {
    case 1:
        the = "menu-background-1.png"_spr;
        break;
    case 2:
        the = "menu-background-2.png"_spr;
        break;
    case 3:
        the = "menu-background-3.png"_spr;
        break;
    case 4:
        the = "menu-background-4.png"_spr;
        break;
    case 5:
        the = "menu-background-5.png"_spr;
        break;
    case 6:
        the = "menu-background-6.png"_spr;
        break;
    case 7:
        the = "menu-background-7.png"_spr;
        break;
    case 8:
        the = "menu-background-8.png"_spr;
        break;
    default:
        the = "square.png";
    }
    log::debug("[MainMenu]: {} {}", the, bgIndex);
    auto d = CCSprite::createWithSpriteFrameName(the.c_str());
    CCSize m = d->getContentSize();
    CCSize m2 = CCDirector::sharedDirector()->getWinSize();
    d->setScale(m2.width/m.width);
    d->setAnchorPoint(ccp(0, 0));
    auto paral = ParallaxContainer::create();
    paral->addChild(d);
    this->addChild(paral);

    auto back = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(MainMenu::debugReturn));
    auto j = CCMenu::create();
    j->addChild(back);
    this->addChildAtPosition(j, Anchor::TopLeft);
    
    auto btnParal = ParallaxContainer::create(0.01f);
    auto g = ButtonSystem::create(logo);
	btnParal->addChild(g);
    this->addChild(btnParal);
    this->setAnchorPoint(ccp(0.5,0.5));
    this->setAnchor(Anchor::Center);
    this->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);

    return true;
}

void MainMenu::onLogoClickIdle() {
    auto logo = getChildOfType<OsuLogo>(this, 0);
    //auto btns = ButtonSystem
}
