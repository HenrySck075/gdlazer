#include "SettingsSection.hpp"
#include "../OverlayColorProvider.hpp"
#include "../../../frameworks/graphics/animations/ContainerActions.hpp"
GDL_NS_START

using namespace frameworks;

///////////////////////////////
///     SettingsSection     ///
///////////////////////////////

bool SettingsSection::init(std::string header, IconConstructor icon) {
  if (!FillFlowContainer::init({
    FillDirection::Vertical,
    geode::Anchor::TopLeft,
    2.f
  })) {
    return false;
  }
  //this->setPadding(8);

  auto provider = OverlayColorProvider::create(OverlayColorScheme::Purple);

  m_header = header; 
  m_icon = icon;
  this->setContentSize({100,100}, Unit::Percent);
  this->addChild(m_headerText = OsuText::create({
    .text = header,
    .fontSize = 18*2
  }));
  this->setAnchorPoint({0,1});
  this->setAnchor(Anchor::TopLeft);
  this->setLogLevel(geode::Severity::Debug);

  this->addChild(m_unfocusedOverlay = CCLayerColor::create(
    provider->Background5().opacity(128)
  ), 999999);
  this->addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    m_unfocusedOverlay->setContentSize(this->getContentSize());
    return true;
  });
  return true;
}

void SettingsSection::balls(float) {
  this->updateLayout();
}

void SettingsSection::onEnter() {
  Container::onEnter();
  scheduleOnce(schedule_selector(SettingsSection::balls),0);
}

void SettingsSection::addSettings(Container* settingsView) {
  this->addChild(settingsView);
  if (m_bRunning) {
    scheduleOnce(schedule_selector(SettingsSection::balls),0);
  }
}

void SettingsSection::focus() {
  m_unfocusedOverlay->runAction(
    CCFadeTo::create(0.5,0)
  );
}
void SettingsSection::unfocus() {
  m_unfocusedOverlay->runAction(
    CCFadeTo::create(0.5,100)
  );
}

////////////////////////////////
///    SettingsSubsection    ///
////////////////////////////////

bool SettingsSubsection::init(std::string header) {
  if (!SettingsSection::init(header, {33, "bigFont.fnt"})) return false;

  m_headerText->setFontSize(16*2);
  m_unfocusedOverlay->setVisible(false);

  return true;
}

GDL_NS_END