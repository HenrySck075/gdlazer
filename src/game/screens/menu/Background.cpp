#include "Background.hpp"
#include "Geode/utils/web.hpp"
#include <chrono>
#include <matjson.hpp>
#include <random>

bool Background::init() {
  Container::init();
  
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setCascadeOpacityEnabled(true);
  setOpacity(255);
  setColor({0,0,0,0});
  setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
  addListener("nodeLayoutUpdate", [this](NodeEvent* ev){
    //auto e = static_cast<NodeLayoutUpdate*>(ev);
    if (m_background) {
      auto s = CCLayer::getContentSize();
      m_background->setPosition(s/2);
      m_background->setContentSize(s);
    }
  });
  return true;
}

std::string Background::roll() {
  std::vector<matjson::Value> out;
  while (true) {
    std::sample(
      m_backgrounds.begin(),
      m_backgrounds.end(),
      std::back_inserter(out),
      1,
      std::mt19937{std::random_device{}()}
    );
    std::string bg = out[0].asString().unwrap();
    if (!m_background) return bg;
    if (bg != m_backgroundName) {
      return bg;
    }
  }
}
void Background::switchBackground() {
  m_backgroundGetListener.setFilter(meow::immediate(roll()));
};
