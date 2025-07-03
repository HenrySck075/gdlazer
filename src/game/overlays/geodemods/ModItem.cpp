#include "ModItem.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../frameworks/graphics/animations/ContainerActions.hpp"
#include "../../graphics/ui/OsuText.hpp"
#include <Geode/ui/LazySprite.hpp>
#include <Geode/ui/GeodeUI.hpp>

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
  setTouchEnabled(true);

  /// Mod icon
  float heightUIKit = processUnit(HEIGHT, Unit::UIKit, false);
  auto logo = LazySprite::create({heightUIKit, heightUIKit}, false);
  logo->setAutoResize(true);

  if (!mod->isInternal()) {
    logo->setLoadCallback([logo](Result<>){
      logo->setAnchorPoint({0,0});
    });
    logo->loadFromFile(dirs::getModRuntimeDir() / mod->getID() / "logo.png");
  } else {
    logo->initWithSpriteFrameName("geode.loader/geode-logo.png");
  }


  auto bg = Container::create();
  bg->setContentSize({HEIGHT+CORNER_RADIUS*2+30, HEIGHT},Unit::UIKit);
  bg->setBorderRadius(CORNER_RADIUS);
  bg->setBackgroundColor(provider->Background2());

  bg->setClippingEnabled(true);
  addChild(bg);
  
  logo->setAnchorPoint({0,0});
  addChild(logo);
  /// Metadata
  auto metaContainer = $verifyPtr(FillFlowContainer::create(FillDirection::Vertical, Anchor::TopLeft));
  metaContainer->setPosition({HEIGHT, 0},Unit::UIKit);
  metaContainer->setBorderRadius(CORNER_RADIUS);
  metaContainer->setClippingEnabled(true);
  metaContainer->setBackgroundColor(provider->Background3());
  {
    auto t = $verifyPtr(OsuText::create({.text = mod->getName(), .fontSize = 25}));
    t->setPadding({0,0,CORNER_RADIUS,0});
    metaContainer->addChild(t);
  }
  {
    auto t = $verifyPtr(OsuText::create({.text = "by " + getDevelopersString(mod->getDevelopers()), .fontSize = 17}));
    t->setPadding({0,0,CORNER_RADIUS,0});
    metaContainer->addChild(t);
  }

  const ccColor4B bgNormal = provider->Background3();
  const ccColor4B bgHover = provider->Background3().lighten(120);
  log::debug("[]: {} {}", bgNormal, bgHover);

  this->addListener<MouseEvent>([metaContainer, bgNormal, bgHover, mod](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click) {
      geode::openInfoPopup(mod);
    }
    else if (e->m_eventType == MouseEventType::Enter) {
      metaContainer->stopAllActions();
      metaContainer->runAction(ContainerTintTo::create(0.25, bgHover));
    }
    else if (e->m_eventType == MouseEventType::Exit) {
      metaContainer->stopAllActions();
      metaContainer->runAction(ContainerTintTo::create(0.25, bgNormal));
    }
    return true;
  });
  addChild(metaContainer);
  metaContainer->setGap(2);
  metaContainer->setAutoResize(false);
  metaContainer->updateLayout();
  //metaContainer->setContentSize({WIDTH - HEIGHT + CORNER_RADIUS, metaContainer->getContentHeight()}, Unit::UIKit, Unit::OpenGL);
  metaContainer->setMinSize({WIDTH - HEIGHT,HEIGHT}); /// what happened why dont this work

  return true;
}

GDL_NS_END