#pragma once

#include <Geode/Geode.hpp>
#include "../../../helpers/MouseEvent.hpp"

using namespace geode::prelude;

class CCNodeHover : public CCNodeRGBA {
private:
	bool m_hoverEnabled = true;
	EventListenerProtocol* m_listener;
	virtual void onMouseEnter() = 0;
	virtual void onMouseExit() = 0;
public:
	bool m_entered = false;
	bool init();

	void setHoverEnabled(bool state) { m_hoverEnabled = state; };
	bool getHoverEnabled() { return m_hoverEnabled; }

	/// <summary>
	/// Because Geode does not automatically destroy event listeners as promised in the docs
	/// </summary>
	void destroyListener() {
		m_listener->~EventListenerProtocol();
	}
};
