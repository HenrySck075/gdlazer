#include "utils.hpp"

float randomFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

bool randomBool() {
	return randomFloat() >= 0.5;
}

/// <summary>
/// im suck at naming so uhm read the code
/// </summary>
/// <param name="node"></param>
/// <returns>the node's bottom-left position</returns>
CCPoint getPositionAnchorAware(CCNode* node) {
	auto pos = node->getPosition();
	auto size = node->getContentSize();
	auto anchor = node->isIgnoreAnchorPointForPosition() ? CCPoint{0,0} : node->getAnchorPoint();

	return CCPoint{ pos.x - size.width * anchor.x,pos.y - size.height * anchor.y };
}


CCRect boundingBoxFromContentSize(CCNode* node) {
	auto size = node->getContentSize();

	auto pos = getPositionAnchorAware(node);
	auto parent = node->getParent();
	while (parent != nullptr) {
		pos += getPositionAnchorAware(parent);
		//log::debug("[boundingBoxFromContentSize]:n pos: {} | parent: {}", pos, parent);
		parent = parent->getParent();
	}
	//log::debug("[boundingBoxFromContentSize]: base size: {}, calculated pos: {}", size, pos);

	return CCRect{ pos.x, pos.y, size.width, size.height };
}

CCRect flipRect(CCRect rect) {
	auto ws = CCDirector::sharedDirector()->getWinSize();
	return CCRect{ rect.getMinX(),ws.height - rect.getMinY() + rect.size.height,rect.size.width, rect.size.height };
}