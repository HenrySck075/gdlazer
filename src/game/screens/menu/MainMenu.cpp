#include "MainMenu.hpp"
#include "OsuLogo.hpp"
#include "ButtonConstants.hpp"

static CCPoint pos {-ButtonSystem::WEDGE_WIDTH*4,BUTTON_AREA_HEIGHT/2};

MainMenu* MainMenu::create() {
  create_class(MainMenu, init);
}

bool MainMenu::init() {
  Screen::init();
  m_title = "Welcome to Geometry Dash!";
  auto logo = OsuLogo::create();

  auto bgParal = ParallaxContainer::create();
  bg = Background::create();
  bgParal->addChild(bg);
  this->addChild(bgParal);
  
  buttonSysParallax = ParallaxContainer::create(0.01f);
  buttonSys = ButtonSystem::create(logo);
  buttonSysParallax->addChild(buttonSys);
  this->addChild(buttonSysParallax);

  addListener("musicStarted", [this](NodeEvent*) {
    bg->switchBackground();
  });
  return true;
}

void MainMenu::onLogoClickIdle() {
  auto logo = getChildOfType<OsuLogo>(this, 0);
  logoMoveAction = CCMoveToModifiable::create(0.5, pos);
  logo->runAction(logoMoveAction);
}
void MainMenu::debugReturn(CCObject *t) { 
  if (GameManager::sharedState()->m_menuLayer==nullptr) {
    log::debug("l");
    return;
  }
  OsuGame::get()->addChild(GameManager::sharedState()->m_menuLayer);
}

