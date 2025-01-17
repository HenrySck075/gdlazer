//https://osu.ppy.sh/api/v2/seasonal-backgrounds
#pragma once

#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"
#include <henrysck075.imagecache/include/ImageCache.hpp>
#include <matjson.hpp>
#include "../../../framework/graphics/sprites/CCResizableSprite.hpp"

using meow = Task<std::string>;

class Background : public Container {
private:
  matjson::Value m_backgrounds = { std::vector<matjson::Value>{} };
  EventListener<meow> m_backgroundGetListener;
  EventListener<web::WebTask> m_seasonalBgsListener;
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
