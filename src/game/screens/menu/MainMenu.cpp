#include "MainMenu.hpp"
#include "OsuLogo.hpp"
#include "ButtonConstants.hpp"
#include "../../graphics/ui/OsuText.hpp"

#include "../../OsuGame.hpp"

GDL_NS_START
using namespace frameworks;
static const CCPoint pos {-ButtonSystem::s_wedgeWidth*4,c_buttonAreaHeight/2};

MainMenu* MainMenu::create() {
  $createClass(MainMenu, init);
}

bool MainMenu::init() {
  Screen::init();
  setTitle("Welcome to Geometry Dash!");
  auto logo = OsuLogo::create();

  auto bgParallax = ParallaxContainer::create();
  m_background = Background::create();
  m_background->setContentSize({100,100},Unit::Percent);
  bgParallax->addChild(m_background);
  addChild(bgParallax);
  
  m_buttonSysParallax = ParallaxContainer::create(0.01f);
  m_buttonSys = ButtonSystem::create(logo);
  m_buttonSys->setAnchor(Anchor::Center);
  m_buttonSysParallax->addChild(m_buttonSys);
  addChild(m_buttonSysParallax);

  m_nowPlayingBox = FillFlowContainer::create({FillDirection::Vertical, Anchor::TopRight});
  m_nowPlayingBox->setAnchorPoint({1,1});

  auto songTitle = OsuText::create({.text = "", .fontSize = 14*1.5f, .alignment = kCCTextAlignmentRight});
  m_nowPlayingBox->addChild(songTitle);
  auto songArtist = OsuText::create({.text = "", .fontSize = 12*1.5f, .alignment = kCCTextAlignmentRight});
  m_nowPlayingBox->addChild(songArtist);
  auto levelInfo = OsuText::create({.text = "", .fontSize = 10*1.5f, .alignment = kCCTextAlignmentRight});
  m_nowPlayingBox->addChild(levelInfo);
  /*
  m_nowPlayingBox->setLayout(
    ColumnLayout::create()
    ->setAxisReverse(true)
    ->setCrossAxisLineAlignment(AxisAlignment::End)
    ->setAxisAlignment(AxisAlignment::End)
    ->setGap(0)
  );
  */
  m_nowPlayingBox->setContentSize({300,40});
  m_nowPlayingBox->setCascadeOpacityEnabled(true);
  m_nowPlayingBox->setOpacity(0);
  addChild(m_nowPlayingBox);
  
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated* ){
    m_nowPlayingBox->setPosition(CCNode::getContentSize()-CCSize{5,5});
    return true;
  });

  addListener<MusicStarted>([this,songTitle,songArtist,levelInfo](MusicStarted*) {
    showNowPlayingText(songTitle, songArtist, levelInfo);
    return true;
  });
  addListener<MusicInfoRequest>([this,songTitle,songArtist,levelInfo](MusicInfoRequest*) {
    showNowPlayingText(songTitle, songArtist, levelInfo, false);
    return true;
  });
  queueInMainThread([this]{
    AudioManager::get()->dispatchMusicInfoEvent(this);
  });

  addListener<MusicEnded>([](MusicEnded*){
    OsuGame::get()->startNextSong();
    return true;
  });

  /////////////////////////////////////////////
  {
    if (auto game = OsuGame::get(false)) game->showToolbar();
  }
  /////////////////////////////////////////////

  return true;
}
void MainMenu::showNowPlayingText(OsuText* songTitle, OsuText* songArtist, OsuText* levelInfo, bool switchBackground) {
  if (switchBackground) m_background->switchBackground();
  auto a = AudioManager::get();
  songTitle->setString(a->getSongName().c_str());
  songArtist->setString(a->getSongAuthor().c_str());
  levelInfo->setString(fmt::format("{} - {}", a->getLevelName(), a->getLevelAuthor()).c_str());

  m_nowPlayingBox->updateLayout();
  CCArray* actions = CCArray::create(
    CCFadeIn::create(0.5),
    CCDelayTime::create(5),
    CCFadeOut::create(0.75),
    nullptr
  );
  if (getActionManager()->numberOfRunningActionsInTarget(actions)!=0) {
    actions->insertObject(CCFadeOut::create(0.25),0);
  }
  m_nowPlayingBox->runAction(CCSequence::create(actions));
}
void MainMenu::onLogoClickIdle() {
  auto logo = this->getChildByType<OsuLogo>(0);
  m_logoMoveAction = CCMoveToModifiable::create(0.5, pos);
  logo->runAction(m_logoMoveAction);
}

void gdlazer::game::MainMenu::onScreenExit(
    frameworks::ScreenTransitionEvent e) {
  // setVisible(false);
  if (e.next == nullptr)
    removeFromParent();
  else {
    setCascadeOpacityEnabled(true);
    m_background->runAction(CCFadeOut::create(2));
    runAction(CCSequence::createWithTwoActions(CCDelayTime::create(2),
                                               CCFadeOut::create(1)));
    m_buttonSys->m_area->hide(m_buttonSys->m_area->getCurrent().value(), true, true);
  }
}
GDL_NS_END
