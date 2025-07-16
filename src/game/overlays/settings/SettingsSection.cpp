#include "SettingsSection.hpp"
#include "../OverlayColorProvider.hpp"
#include "../../../frameworks/graphics/animations/ContainerActions.hpp"
GDL_NS_START

using namespace frameworks;

///////////////////////////////
///     SettingsSection     ///
///////////////////////////////

bool SettingsSection::init(std::string header, IconConstructor icon) {
  if (!Container::init()) {
    return false;
  }
  this->setPadding(8);

  auto provider = OverlayColorProvider::create(OverlayColorScheme::Purple);

  m_header = header; 
  m_icon = icon;
  this->addChild(m_content = FillFlowContainer::create({
    FillDirection::Vertical,
    geode::Anchor::TopLeft,
    2.f
  }));
  this->setContentWidth(100, Unit::Percent);
  m_content->setContentWidth(100, Unit::Percent);

  m_content->addChild(m_headerText = OsuText::create({
    .text = header,
    .fontSize = 18*2
  }));

  this->addChild(m_unfocusedOverlay = CCLayerColor::create(
    provider->Background5().opacity(128)
  ));
  this->addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    m_unfocusedOverlay->setContentSize(this->getContentSize());
    return true;
  });
  m_content->addListener<NodeSizeUpdated>([this](NodeSizeUpdated*){
    log::debug("{}: galls {}", this, m_content->getContentSize());
    setContentHeight(m_content->getContentHeight());
    return true;
  });

  return true;
}

void SettingsSection::onEnter() {
  Container::onEnter();
  //m_content->updateLayout();
}

void SettingsSection::addSettings(Container* settingsView) {
  m_content->addChild(settingsView);
  if (m_bRunning) m_content->updateLayout();
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

  return true;
}

GDL_NS_END