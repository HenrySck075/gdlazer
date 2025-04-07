#pragma once

#include "../../frameworks/screens/Screen.hpp"

GDL_NS_START
class PlaygroundScreen : public GDF_NS::Screen {
public:
  static PlaygroundScreen* create() {
    auto ret = new PlaygroundScreen();
    if (ret && ret->init()) {
      ret->autorelease();
      return ret;
    } else {
      delete ret;
      return nullptr;
    }
  };
  bool init() override;
};
GDL_NS_END
  