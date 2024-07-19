#pragma once

#include <Geode/Geode.hpp>
#include "../../../helpers/MouseEvent.hpp"

using namespace geode::prelude;

class CCNodeHover : public CCNodeRGBA {
private:
	EventListenerProtocol* m_listener;
	void onMouseEnter();
	void onMouseExit();
public:
  bool init();
};
