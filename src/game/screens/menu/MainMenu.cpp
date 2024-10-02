#include "MainMenu.hpp"
#include "OsuLogo.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"

bool MainMenu::init() {
    Screen::init();
    m_title = "Welcome to Geometry Dash!";
    auto logo = OsuLogo::create();

    auto bgParal = ParallaxContainer::create();
    bg = Background::create();
    bgParal->addChild(bg);
    this->addChild(bgParal);

    auto back = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(MainMenu::debugReturn));
    auto test = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(MainMenu::testDispatch));

    auto j = CCMenu::create();
    j->addChild(back);
    j->addChild(test);
    j->setLayout(RowLayout::create()->setGap(3)->setAxisAlignment(AxisAlignment::End));
    j->updateLayout();
    this->addChildAtPosition(j, Anchor::TopLeft);
    
    auto btnParal = ParallaxContainer::create(0.01f);
    buttonSys = ButtonSystem::create(logo);
    btnParal->addChild(buttonSys);
    this->addChild(btnParal);

    return true;
}

void MainMenu::onLogoClickIdle() {
    auto logo = getChildOfType<OsuLogo>(this, 0);
    //auto btns = ButtonSystem
}
