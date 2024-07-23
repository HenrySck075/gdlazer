#include "ParallaxContainer.hpp"

bool ParallaxContainer::init(float parallaxAmount, bool scale) {
	CCLayer::init();
	this->m_parallaxAmount = parallaxAmount;
	this->director = CCDirector::sharedDirector();
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(ccp(0.5, 0.5));
	this->setPosition(director->getWinSize() / 2);
	if (scale) this->setScale(1 + abs(parallaxAmount));

    m_listener = this->template addEventListener<MouseFilter>([this](MouseType type, CCPoint location) {
		// iykyk
		if (type == MouseType::Move) { updateParallax(location); }
	}, false);
	return true;
}

void ParallaxContainer::updateParallax(CCPoint& cursorPos) {
	auto ws = director->getWinSize()/2;
	auto dist = (cursorPos - ws) * m_parallaxAmount;
	this->setPosition(ws + dist);
}