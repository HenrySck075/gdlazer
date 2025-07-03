#include "ToolbarButtons.hpp"
#include "ToolbarConstants.hpp"
#include "../../OsuGame.hpp"
#include "../../../frameworks/input/events/KeyEvent.hpp"

GDL_NS_START

using namespace frameworks;

/**
 * ToolbarSettingsButton
 */

bool ToolbarSettingsButton::init() {
  setID("settings");
  m_settingsOverlay = SettingsPanel::create();
  addListener<KeyEvent>([this](KeyEvent *e) {
    if (e->m_modifiers.ctrl && e->m_key == enumKeyCodes::KEY_O)
      select();

    return true;
  });
  return ToolbarToggleButton::init(OsuIcon::Settings, "settings", "the", AxisAlignment::Start, "SettingsPanel");
}

void ToolbarSettingsButton::select() {
  //OsuGame::get()->showSettings();
  ToolbarToggleButton::select();
  OsuGame::get()->pushOverlay(m_settingsOverlay);
}

void ToolbarSettingsButton::deselect() {
  //OsuGame::get()->hideSettings();
  ToolbarToggleButton::deselect();
  OsuGame::get()->popOverlay(m_settingsOverlay);
}


/**
 * ToolbarModDisableButton
 */
/*
bool ToolbarModDisableButton::init() { 
  // for why im not hooking/patching ModsLayer to override its onBack to pop the scene,
  // idk
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click)  {
      Mod::get()->disable();
      geode::utils::game::restart();
    }
    return true;
  });
  return ToolbarButton::init(
    OsuIcon::CrossCircle,
    "(Prerelease feature) Disable mod",
    "disable this mf",
    AxisAlignment::End
  );
}
*/

/**
 * ToolbarMusicButton
 */

bool ToolbarMusicButton::init() {
  setID("music");
  o = NowPlayingOverlay::create();
  o->retain();
  return ToolbarToggleButton::init(
    OsuIcon::Music, 
    "now playing", 
    "manage the currently playing track (F6)", 
    AxisAlignment::End,
    "NowPlayingOverlay"
  );
}

void ToolbarMusicButton::select() {
  ToolbarToggleButton::select();
  OsuGame::get()->pushOverlay(o);
}

void ToolbarMusicButton::deselect() {
  ToolbarToggleButton::deselect();
  OsuGame::get()->popOverlay(o);
}


bool ToolbarNativeSettingsButton::init() {
  setID("native-settings");
  addListener<MouseEvent>([this](MouseEvent* e) {
    if (e->m_eventType == MouseEventType::Click && isMouseEntered()) {
      // Open native settings
      MenuLayer::get()->onOptions(nullptr);
    }
    return true;
  });
  return ToolbarButton::init(OsuIcon::Settings, "settings (gd)", "opens gd settings (temporary)", AxisAlignment::End);
}

/**
 * ToolbarGeodeButton
 */
bool ToolbarGeodeButton::init() {
  return ToolbarToggleButton::init(
    OsuIcon::Maintenance,
    "mods list",
    "manages geode mods (F12)",
    AxisAlignment::End,
    "Geode"
  );
}

void ToolbarGeodeButton::select() {
  (m_modsLayer = ModsOverlay::create())->show();
}
void ToolbarGeodeButton::deselect() {
  if (m_modsLayer) m_modsLayer->hide();
}
int getFrameIcon(IconType type) {
  auto gManager = GameManager::get();
  switch(type) {
    default: return gManager->m_playerFrame;
    case IconType::Ship: return gManager->m_playerShip;
    case IconType::Ball: return gManager->m_playerBall;
    case IconType::Ufo: return gManager->m_playerBird;
    case IconType::Wave: return gManager->m_playerDart;
    case IconType::Robot: return gManager->m_playerRobot;
    case IconType::Spider: return gManager->m_playerSpider;
    case IconType::Swing: return gManager->m_playerSwing; 
    case IconType::Jetpack: return gManager->m_playerJetpack;
  }
}
bool ToolbarUserButton::init() {
  setID("user");
  if (!ToolbarButton::init(OsuIcon::Music, "",
      "",
      AxisAlignment::End)) return false;
  removeChild(m_iconSprite);
  setContentSize({ToolbarConstants::c_height*4,ToolbarConstants::c_height},Unit::UIKit,Unit::UIKit);

  auto flowContainer = FillFlowContainer::create();
  flowContainer->setChildAnchor(Anchor::Left);
  flowContainer->setGap(2);
  flowContainer->setAutoResize(true);
  flowContainer->setAnchor(Anchor::Center);
  flowContainer->setAnchorPoint({0.5,0.5});

  auto man = GameManager::get();
  auto username = OsuText::create({.text = man->m_playerName, .fontSize = 15});
  username->setMaxSize({ToolbarConstants::c_height*2.f, ToolbarConstants::c_height});
  username->setAnchor(Anchor::Left);
  flowContainer->addChild(username);

  auto icon = SimplePlayer::create(0);
  icon->updatePlayerFrame(getFrameIcon(man->m_playerIconType), man->m_playerIconType);
  icon->setColor(man->colorForIdx(man->getPlayerColor()));
  icon->setSecondColor(man->colorForIdx(man->getPlayerColor2()));
  icon->setGlowOutline(man->colorForIdx(man->getPlayerGlowColor()));
  icon->enableCustomGlowColor(man->colorForIdx(man->getPlayerGlowColor()));
  if(!man->getPlayerGlow()) icon->disableGlowOutline();

  
  auto iconWrapper = Container::create();
  
  iconWrapper->addChild(icon);
  float deadass = static_cast<CCNode*>(icon->getChildren()->objectAtIndex(0))->getContentHeight();
  iconWrapper->addListener<NodeSizeUpdated>([icon, iconWrapper, deadass, this](NodeSizeUpdated* e) {
    icon->setScale(processUnit(ToolbarConstants::c_height-6, Unit::UIKit, false) / deadass);
    icon->setPositionY(getContentHeight()/2);
    return true;
  });
  iconWrapper->setContentSize({ToolbarConstants::c_height, ToolbarConstants::c_height}, Unit::UIKit);
  static_cast<CCNode*>(icon->getChildren()->objectAtIndex(0))->setAnchorPoint({0,0.5});
  icon->setAnchorPoint({0,0.5});
  //iconWrapper->setPadding({2});
  iconWrapper->setAnchor(Anchor::Left);

  flowContainer->addChild(iconWrapper);

  flowContainer->updateLayout();
  username->inner()->limitLabelWidth(username->getContentWidth(), 1, 0.02);

  addChild(flowContainer);

  addListener<MouseEvent>([this](MouseEvent* e) {
    if (e->m_eventType == MouseEventType::Click && isMouseEntered()) {
      MenuLayer::get()->onMyProfile(this);
    }
    return true;
  });

  return true;
}
GDL_NS_END
