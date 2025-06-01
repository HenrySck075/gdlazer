#include "ModItem.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../graphics/ui/OsuText.hpp"
#include <Geode/ui/LazySprite.hpp>

GDL_NS_START
using namespace frameworks;
GeodeModItem* GeodeModItem::create(geode::Mod* mod, OverlayColorProvider* provider) {
  $createClass(GeodeModItem, init, mod, provider);
}

std::string getDevelopersString(const std::vector<std::string>& developers) {
  if (developers.size() == 1) return developers[0];
  return developers[0] + " & " + std::to_string(developers.size()-1) + " more";
}

bool GeodeModItem::init(geode::Mod* mod, OverlayColorProvider* provider) {
  if (!Container::init()) return false;
  setContentSize({WIDTH, HEIGHT}, Unit::UIKit);
  setBorderRadius(CORNER_RADIUS);
  setClippingEnabled(true);

  /// Mod icon
  float heightUIKit = processUnit(HEIGHT, Unit::UIKit, false);
  auto logo = geode::LazySprite::create({heightUIKit, heightUIKit},false);
  logo->loadFromFile(geode::dirs::getModRuntimeDir() / mod->getID() / "logo.png");
  addChild(logo);

  /// Metadata
  auto metaContainer = $verifyPtr(FillFlowContainer::create(FillDirection::Vertical));
  metaContainer->setPosition({HEIGHT - CORNER_RADIUS, 0});
  metaContainer->setBorderRadius(CORNER_RADIUS);
  metaContainer->setBackgroundColor(provider->Background2());
  metaContainer->addChild($verifyPtr(OsuText::create(mod->getName(), FontType::Regular, 16)));
  metaContainer->addChild($verifyPtr(OsuText::create("by " + getDevelopersString(mod->getDevelopers()), FontType::Regular, 11)));
  addChild(metaContainer);
  metaContainer->updateLayout();
  metaContainer->setContentSize({WIDTH - HEIGHT + CORNER_RADIUS,HEIGHT}, Unit::UIKit);

  return true;
}

GDL_NS_END