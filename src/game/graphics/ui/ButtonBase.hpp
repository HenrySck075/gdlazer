#pragma once

#include <Geode/Geode.hpp>
#include "../../graphics/ui/CCNodeHover.hpp"
#include "Geode/utils/MiniFunction.hpp"

using namespace geode::prelude;
using ButtonCallback = geode::utils::MiniFunction<void(CCNode*)> ;

// button base
class ButtonBase : public CCNodeHover, public CCTouchDelegate {
private:

	MiniFunction<void(CCNode*)> clickCallback;
	const char* clickSfx;

	bool touchStartInBounds = false;
	bool holding = false;

	EventListenerProtocol* m_clickListener;

	//bool ccTouchBegan(CCTouch* t, CCEvent* idc) override;
	//void ccTouchEnded(CCTouch* t, CCEvent* idc) override;

public:
	void setClickSFX(const char* sfx) { clickSfx = sfx; };
	/// <param name="clickCb">| the</param>
	/// <param name="self">| the node to be used as the clickCb parameter</param>
	bool init(const char* clickSfx, ButtonCallback clickCb, CCNode* self);
};

