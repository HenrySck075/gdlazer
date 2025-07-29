#include "SettingsPanel.hpp"
#include "settings/SidebarIconButton.hpp"
#include "../OsuGame.hpp"
#include "../../frameworks/graphics/animations/ActionEase.hpp"
#include "../../frameworks/graphics/animations/ContainerActions.hpp"
#include "OverlayColorProvider.hpp"

GDL_NS_START

using namespace frameworks;

bool SettingsPanel::init() {
  if (!OsuOverlayContainer::init()) return false;
  this->setBackgroundColor({0,0,0,155});
  this->setName("SettingsPanel");
  this->setTitle("Settings");
  auto provider = OverlayColorProvider::to(this, OverlayColorScheme::Purple);

  m_mainPanel = Container::create();
  m_mainPanel->setBackgroundColor(provider->Background4());
  m_mainPanel->setContentSize({c_panelWidth,100},Unit::UIKit,Unit::Percent);
  m_mainPanel->setPosition({-c_panelWidth,0},Unit::UIKit,Unit::OpenGL);
  m_main->addChild(m_mainPanel);
  
  m_sidebar = SettingsSidebar::create();
  m_sidebar->setPosition({-SettingsSidebar::c_expandedWidth,0},Unit::UIKit,Unit::OpenGL);
  m_main->addChild(m_sidebar);

  m_main->setContentSize({c_width,100},Unit::UIKit,Unit::Percent);
  m_main->setAnchor(Anchor::Left);
  m_main->setAnchorPoint({0,0.5});

  m_sections.inner()->addObjectsFromArray(createSections().inner()); 
  m_sectionsContainer = SettingsSections::create();
  // ???
  m_sectionsContainer->setContentSize({100,100}, Unit::Percent);
  for (auto section : m_sections) {
    m_sectionsContainer->addSection(section);
  }
  m_sectionsContainer->onSectionSelect(0, m_sections[0]);
  m_mainPanel->addChild(m_sectionsContainer);
  for (auto sidebarButton : createSidebarButtons()) {
    m_sidebar->addSidebarItem(sidebarButton);
  }
  queueInMainThread([this]{
    m_sidebar->updateLayout();
  });


  setContentSize({100,100}, Unit::Percent);

  return true;
}

CCArrayExt<SidebarButton> SettingsPanel::createSidebarButtons() {
  CCArrayExt<SidebarButton> buttons;
  auto provider = OverlayColorProvider::of(this);
  for (auto section : m_sections) {
    auto button = SidebarIconButton::create({
      section->getHeader(), 
      section->getIcon(),
      provider
    });
    /*
    button->setOnClickCallback([this, &section](SidebarButton*){
      m_sectionsContainer->selectSection(section);
    });
    */
    buttons.push_back(button);
  }
  return buttons;
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
GDL_NS_END
#include "settings/sections/AudioSection.hpp"
GDL_NS_START
CCArrayExt<SettingsSection> SettingsPanel::createSections() {
  return CCArray::create(
    AudioSection::create(),
    nullptr
  );
}

/**
 * SettingsSections
 */

void SettingsSections::onSectionSelect(Container* old, Container* new_) {
  SectionsContainer::onSectionSelect(old, new_);
  if (old) static_cast<SettingsSection*>(old)->unfocus();
  if (new_) static_cast<SettingsSection*>(new_)->focus();
};
bool SettingsSections::init() {
  if (!SectionsContainer::init())
    return false;
  setContentSize({100,100}, Unit::Percent);
  return true;
}

GDL_NS_END
