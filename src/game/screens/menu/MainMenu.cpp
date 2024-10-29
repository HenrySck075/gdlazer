#include "MainMenu.hpp"
#include "OsuLogo.hpp"
#include "ButtonConstants.hpp"
#include "../../graphics/ui/OsuText.hpp"

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

  joe = CCLayerRGBA::create();
  joe->setAnchorPoint({1,1});
  joe->ignoreAnchorPointForPosition(false);

  auto songTitle = OsuText("",FontType::Regular,16,kCCTextAlignmentRight);
  joe->addChild(songTitle);
  auto songArtist = OsuText("",FontType::Regular,12,kCCTextAlignmentRight);
  joe->addChild(songArtist);
  joe->addChild(joe);
  joe->setLayout(
    ColumnLayout::create()
  );
  joe->setContentSize({300,120});
  joe->updateLayout();
  joe->setCascadeOpacityEnabled(true);
  
  addListener("nodeLayoutUpdate", [this](NodeEvent* ){
    joe->setPosition(CCNode::getContentSize()-CCSize{5,5});
  });

  addListener("musicStarted", [this,songTitle,songArtist](NodeEvent*) {
    bg->switchBackground();
    auto a = AudioManager::get();
    songTitle->setString(a->getSongName().c_str());
    songArtist->setString(a->getSongAuthor().c_str());

    CCArray* actions = CCArray::create(
      CCFadeIn::create(0.25),
      CCDelayTime::create(5),
      CCFadeOut::create(0.75),
      nullptr
    );
    if (getActionManager()->numberOfRunningActionsInTarget(actions)!=0) {
      actions->insertObject(CCFadeOut::create(0.25),0);
    }
    joe->runAction(CCSequence::create(actions));
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

