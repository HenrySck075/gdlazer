#include "SettingsSection.hpp"
#include "../OverlayColorProvider.hpp"


GDL_NS_START

using namespace frameworks;

bool SettingsSection::init(std::string header, IconConstructor icon) {
  if (!Container::init()) {
    return false;
  }

  auto provider = OverlayColorProvider::create(OverlayColorScheme::Purple);

  m_header = header; 
  m_icon = icon;

  m_content = FillFlowContainer::create(FillDirection::Vertical);
  m_content->setPadding(10);
  this->addChild(m_content);

  m_unfocusedOverlay = CCLayerColor::create(provider->Background5().opacity(128));
  addChild(m_unfocusedOverlay);
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated* event) {
    m_unfocusedOverlay->setContentSize(this->getContentSize());
    return true;
  });


  return true;
}

void SettingsSection::addSettingsContainer(Container* settingsView) {
  m_content->addChild(settingsView);
}


GDL_NS_END