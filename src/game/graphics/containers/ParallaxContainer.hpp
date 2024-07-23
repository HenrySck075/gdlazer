#include "../../../helpers/MouseEvent.hpp"
#include "../../../utils.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// why are they using ease out quint just for parallax am i dumb
class ParallaxContainer : public CCLayer {
private:
	// used to determine the center position
	CCDirector* director;
	EventListenerProtocol* m_listener;
	float m_parallaxAmount;
public:
	static ParallaxContainer* create(float parallaxAmount = 0.02f, bool scale = true) {
		create_class(ParallaxContainer, init, parallaxAmount, scale);
	}
	bool init(float parallaxAmount, bool scale);
	void updateParallax(CCPoint& cursorPos);
};