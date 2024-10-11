#include "MainMenu.hpp"
#include "OsuLogo.hpp"
#include "ButtonConstants.hpp"

static CCPoint pos {-ButtonSystem::WEDGE_WIDTH*4,BUTTON_AREA_HEIGHT/2};

bool MainMenu::init() {
    Screen::init();
    m_title = "Welcome to Geometry Dash!";
    auto logo = OsuLogo::create();

    auto bgParal = ParallaxContainer::create();
    bg = Background::create();
    bgParal->addChild(bg);
    this->addChild(bgParal);

    auto back = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(MainMenu::debugReturn));

    auto j = CCMenu::create();
    j->addChild(back);
    j->setLayout(RowLayout::create()->setGap(3)->setAxisAlignment(AxisAlignment::End));
    j->updateLayout();
    this->addChildAtPosition(j, Anchor::TopLeft);
    
    buttonSysParallax = ParallaxContainer::create(0.01f);
    buttonSys = ButtonSystem::create(logo);
    buttonSysParallax->addChild(buttonSys);
    this->addChild(buttonSysParallax);

    return true;
}

void MainMenu::onLogoClickIdle() {
    auto logo = getChildOfType<OsuLogo>(this, 0);
    logoMoveAction = CCMoveToModifiable::create(0.5, pos);
    logo->runAction(logoMoveAction);
}
void MainMenu::debugReturn(CCObject *t) { 
    OsuGame::get()->addChild(GameManager::get()->m_menuLayer);
}

