#pragma once

#include "../../../utils.hpp"
#include <Geode/Geode.hpp>
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../framework/input/events/MouseEvent.hpp"

using namespace geode::prelude;

// why are they using ease out quint just for parallax am i dumb
class ParallaxContainer final : public Container {
private:
    // used to determine the center position
    CCDirector* director;
    EventListenerProtocol* m_listener;
    float m_parallaxAmount;
    bool parallax = true;
    CCPoint baseLocation = CCPoint(0,0);
public:
    static ParallaxContainer* create(float parallaxAmount = 0.02f, bool scale = true) {
        create_class(ParallaxContainer, init, parallaxAmount, scale);
    }
    bool init(float parallaxAmount, bool scale);
    void updateParallax(const CCPoint& cursorPos);
    void onMouseMove(MouseEvent* event) {
        updateParallax(event->position); 
    }
};
