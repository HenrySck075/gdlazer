#include "OsuGame.hpp"

GDL_NS_START
geode::Ref<OsuGame> OsuGame::get(bool create) {
  return getAs<OsuGame>(create);
};
GDL_NS_END