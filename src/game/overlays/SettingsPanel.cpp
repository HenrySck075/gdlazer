#include "SettingsPanel.hpp"
#include "../OsuGame.hpp"
#include "../overlays/toolbar/ToolbarToggleButton.hpp"
#include "../../frameworks/graphics/CCEase2.hpp"
#include "../../frameworks/graphics/containers/ContainerActions.hpp"
#include "OverlayColorProvider.hpp"

GDL_NS_START

using namespace frameworks;

bool SettingsPanel::init() {
  if (!OsuOverlayContainer::init()) return false;
  setBackgroundColor({0,0,0,155});
  setName("SettingsPanel");
  setTitle("Settings");

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

  setContentSize({100,100}, Unit::Percent);

  return true;
}

void SettingsPanel::onOpen() {
  log::debug("o");
  stopAllActions();
  m_sidebar->stopAllActions();
  m_mainPanel->stopAllActions();
  m_sidebar->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(SettingsPanel::c_transitionLength,{0,0}), Easing::OutQuint
  ));
  m_mainPanel->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(SettingsPanel::c_transitionLength,{SettingsSidebar::c_expandedWidth,0}), Easing::OutQuint
  ));
  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  if (s->getActionByTag(7)) s->stopActionByTag(7);
  s->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(
      SettingsPanel::c_transitionLength, 
      {SettingsSidebar::c_expandedWidth/4.f,0}
    ), Easing::OutQuint
  ))->setTag(7);
}
void SettingsPanel::onClose() {
  log::debug("c");
  stopAllActions();
  m_sidebar->stopAllActions();
  m_mainPanel->stopAllActions();
  runAction(
    CCDelayTime::create(SettingsPanel::c_transitionLength+0.1)
  );
  m_sidebar->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(
      SettingsPanel::c_transitionLength,
      {-SettingsSidebar::c_expandedWidth,0}
    ), Easing::OutQuint
  ));
  m_mainPanel->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(SettingsPanel::c_transitionLength,{-c_panelWidth,0}), Easing::OutQuint
  ));

  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  s->stopActionByTag(7);
  s->runAction(frameworks::ActionEase::create(
    ContainerMoveTo::create(SettingsPanel::c_transitionLength, {0,0}), Easing::OutQuint
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
