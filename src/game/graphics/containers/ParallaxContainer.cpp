#include "ParallaxContainer.hpp"
#include "../../../helpers/MouseEvent.hpp"

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
	Container::init();
	this->m_parallaxAmount = parallaxAmount;
	this->director = CCDirector::sharedDirector();
	//this->setAnchorPoint(ccp(0.5, 0.5));
	//this->setPosition(director->getWinSize() / 2);
	this->setAnchor(Anchor::Center);
	//this->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
	if (scale) this->setScale(1 + abs(parallaxAmount));

    m_listener = this->template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
		// iykyk
		if (type == MouseType::Move) { 
			updateParallax(location); 
			return true;
		}
		return false;
	}, false);
	//updateParallax(director->convertToGL(director->getOpenGLView()->getMousePosition()));
	return true;
}

void ParallaxContainer::updateParallax(const CCPoint& cursorPos) {
	auto ws = director->getWinSize()/2;
	auto dist = (cursorPos - ws) * m_parallaxAmount;
	dist.y = -dist.y;
	this->setPosition(dist);
}
