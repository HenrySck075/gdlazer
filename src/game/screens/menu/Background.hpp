//https://osu.ppy.sh/api/v2/seasonal-backgrounds
#pragma once

#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"
#include <henrysck075.imagecache/include/ImageCache.hpp>
#include <matjson.hpp>
#include "../../graphics/CCResizableSprite.hpp"

using meow = Task<std::string>;

class Background : public Container {
private:
  EventListener<meow> m_backgroundGetListener;
  EventListener<web::WebTask> m_seasonalBgsListener;
  std::string roll(matjson::Array arr);
  CCResizableSprite* m_background;
  bool m_rescale = false;
public:
    static Background* create() {
        create_class(Background, init);
    };
    bool init();
    void setOpacity(GLubyte opacity) override {
      m_background->setOpacity(opacity);
    }
};
