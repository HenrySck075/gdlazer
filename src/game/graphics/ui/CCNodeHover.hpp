#pragma once

#include <Geode/Geode.hpp>
#include "../../../helpers/MouseEvent.hpp"

using namespace geode::prelude;

class CCNodeHover : virtual public CCNodeRGBA {
private:

	EventListenerProtocol* m_listener;
	virtual void onMouseEnter() = 0;
	virtual void onMouseExit() = 0;
public:
  bool m_entered = false;
  bool init();
};
