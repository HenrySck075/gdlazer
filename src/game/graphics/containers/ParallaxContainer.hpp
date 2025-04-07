#pragma once

#include "../../../utils.hpp"
#include <Geode/Geode.hpp>
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/input/events/MouseEvent.hpp"

using namespace geode::prelude;

GDL_NS_START
// why are they using ease out quint just for parallax am i dumb
class ParallaxContainer final : public GDF_NS::Container {
private:
  // used to determine the center position
  CCDirector* m_director;
  EventListenerProtocol* m_listener;
  float m_parallaxAmount;
  bool m_parallax = true;
  CCPoint baseLocation = CCPoint(0,0);
public:
  static ParallaxContainer* create(float parallaxAmount = 0.02f, bool scale = true) {
    $create_class(ParallaxContainer, init, parallaxAmount, scale);
  }
  bool init(float parallaxAmount, bool scale);
  void updateParallax(const CCPoint& cursorPos);
};
GDL_NS_END