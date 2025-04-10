#include "SettingsPanel.hpp"
#include "../OsuGame.hpp"
#include "../overlays/toolbar/ToolbarToggleButton.hpp"
#include <henrysck075.easings/include/easings.hpp>
#include "OverlayColorProvider.hpp"

const float SettingsPanel::sidebar_width = SettingsSidebar::EXPANDED_WIDTH;
const float SettingsPanel::TRANSITION_LENGTH = 0.6;
const float SettingsPanel::PANEL_WIDTH = 400;
const float SettingsPanel::WIDTH = SettingsPanel::sidebar_width + SettingsPanel::PANEL_WIDTH;

bool SettingsPanel::init() {
  if (!OsuOverlayContainer::init()) return false;
  setColor({0,0,0});
  name = "SettingsPanel";

  mainPanel = Container::create();
  mainPanel->setColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background4());
  mainPanel->setContentSizeWithUnit({PANEL_WIDTH,100},Unit::UIKit,Unit::Percent);
  mainPanel->setPositionWithUnit({-PANEL_WIDTH,0},Unit::UIKit,Unit::OpenGL);
  mainPanel->setOpacity(255);
  main->addChild(mainPanel);
  
  sidebar = SettingsSidebar::create();
  sidebar->setPositionWithUnit({-SettingsSidebar::EXPANDED_WIDTH,0},Unit::UIKit,Unit::OpenGL);
  main->addChild(sidebar);

  main->setContentSizeWithUnit({WIDTH,100},Unit::UIKit,Unit::Percent);
  main->setAnchor(Anchor::Left);
  main->setAnchorPoint({0,0.5});

  return true;
}

void SettingsPanel::onOpen() {
  stopAllActions();
  sidebar->stopAllActions();
  sidebar->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH,{0,0}),5
  ));
  mainPanel->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH,{SettingsSidebar::EXPANDED_WIDTH,0}),5
  ));
  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  if (s->getActionByTag(7)) s->stopActionByTag(7);
  s->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(
      SettingsPanel::TRANSITION_LENGTH, 
      {(float)SettingsSidebar::EXPANDED_WIDTH/4,0}
    ),5
  ))->setTag(7);
}
void SettingsPanel::onClose() {
  stopAllActions();
  sidebar->stopAllActions();
  runAction(
    CCDelayTime::create(SettingsPanel::TRANSITION_LENGTH)
  );
  sidebar->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(
      SettingsPanel::TRANSITION_LENGTH,
      {-SettingsSidebar::EXPANDED_WIDTH,0}
    ),5
  ));
  mainPanel->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH,{-PANEL_WIDTH,0}),5
  ));

  auto s = OsuGame::get()->getChildByIDRecursive("screens");
  s->stopActionByTag(7);
  s->runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH, {0,0}),5
  ))->setTag(7);
}

/**
 * SettingsSections
 */

void SettingsSections::onSectionSelect(Container* old, Container* new_) {
  SectionsContainer::onSectionSelect(old, new_);
  old->setOpacity(200);
  new_->setOpacity(255);
};
