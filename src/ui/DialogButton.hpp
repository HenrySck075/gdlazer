#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"

using namespace geode::prelude;

class DialogButton : public CCMenuItem {
private:
	EventListener<MouseMoveFilter> m_listener = {
		this, 
	};

	void onMouse
};

#endif