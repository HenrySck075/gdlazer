#include "SettingsPanel.hpp"
#include "../OsuGame.hpp"
#include "../overlays/toolbar/ToolbarToggleButton.hpp"
#include "../../frameworks/graphics/CCEase2.hpp"
#include "OverlayColorProvider.hpp"

GDL_NS_START

const float SettingsPanel::c_sidebarWidth = SettingsSidebar::c_expandedWidth;
const float SettingsPanel::c_transitionLength = 0.6;
const float SettingsPanel::c_panelWidth = 400;
const float SettingsPanel::c_width = SettingsPanel::c_sidebarWidth + SettingsPanel::c_panelWidth;

using namespace frameworks;

bool SettingsPanel::init() {
  if (!OsuOverlayContainer::init()) return false;
  setColor({0,0,0});
  setName("SettingsPanel");

  m_mainPanel = Container::create();
  m_mainPanel->setColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background4());
  m_mainPanel->setContentSize({c_panelWidth,100},Unit::UIKit,Unit::Percent);
  m_mainPanel->setPosition({-c_panelWidth,0},Unit::UIKit,Unit::OpenGL);
  m_mainPanel->setOpacity(255);
  m_main->addChild(m_mainPanel);
  
  m_sidebar = SettingsSidebar::create();
  m_sidebar->setPosition({-SettingsSidebar::c_expandedWidth,0},Unit::UIKit,Unit::OpenGL);
  m_main->addChild(m_sidebar);

  m_main->setContentSize({c_width,100},Unit::UIKit,Unit::Percent);
  m_main->setAnchor(Anchor::Left);
  m_main->setAnchorPoint({0,0.5});

  return true;
}

void SettingsPanel::onOpen() {
  stopAllActions();
  m_sidebar->stopAllActions();
  m_sidebar->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::c_transitionLength,{0,0}),5
  ));
  m_mainPanel->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::c_transitionLength,{SettingsSidebar::c_expandedWidth,0}),5
  ));
  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  if (s->getActionByTag(7)) s->stopActionByTag(7);
  s->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(
      SettingsPanel::c_transitionLength, 
      {(float)SettingsSidebar::c_expandedWidth/4,0}
    ),5
  ))->setTag(7);
}
void SettingsPanel::onClose() {
  stopAllActions();
  m_sidebar->stopAllActions();
  runAction(
    CCDelayTime::create(SettingsPanel::c_transitionLength)
  );
  m_sidebar->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(
      SettingsPanel::c_transitionLength,
      {-SettingsSidebar::c_expandedWidth,0}
    ),5
  ));
  m_mainPanel->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::c_transitionLength,{-c_panelWidth,0}),5
  ));

  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  s->stopActionByTag(7);
  s->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::c_transitionLength, {0,0}),5
  ))->setTag(7);
}

/**
 * SettingsSections
 */

void SettingsSections::onSectionSelect(Container* new_) {
  SectionsContainer::onSectionSelect(new_);
  m_currentSection.get()->setOpacity(200);
  new_->setOpacity(255);
};

GDL_NS_END
