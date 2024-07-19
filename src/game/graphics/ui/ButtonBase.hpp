#pragma once

#include <Geode/Geode.hpp>
#include "../../graphics/ui/CCNodeHover.hpp"
#include "Geode/utils/MiniFunction.hpp"

using namespace geode::prelude;
typedef geode::utils::MiniFunction<void(CCNode*)> ButtonCallback;

// button base
class ButtonBase : public CCNodeHover, public CCTouchDelegate {
private:
  ButtonCallback clickCallback;

  bool touchStartInBounds = false;
  bool holding = false;

	EventListenerProtocol* m_listener;

	void onMouseEnter() override;
	void onMouseExit() override;

  bool ccTouchBegan(CCTouch* t, CCEvent* idc) override;
  void ccTouchEnded(CCTouch* t, CCEvent* idc) override;

public: 
	bool init(ButtonCallback clickCb);
};

