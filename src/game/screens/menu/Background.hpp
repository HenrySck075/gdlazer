//https://osu.ppy.sh/api/v2/seasonal-backgrounds
#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include <henrysck075.imagecache/include/ImageCache.hpp>
#include <matjson.hpp>
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"
#include "../../../macro.h"

using meow = Task<std::string>;

GDL_NS_START
class Background : public GDF_NS::Container {
private:
  matjson::Value m_backgrounds = { std::vector<matjson::Value>{} };
  geode::EventListener<meow> m_backgroundGetListener;
  geode::EventListener<web::WebTask> m_seasonalBgsListener;
  std::string roll();
  CCResizableSprite* m_background;
  gd::string m_backgroundName;
  bool m_rescale = false;
public:
  static Background* create() {
    $create_class(Background, init);
  };
  bool init();
  void switchBackground();
  
};
GDL_NS_END