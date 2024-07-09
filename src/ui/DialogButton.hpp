#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"

using namespace geode::prelude;

class DialogButton : public CCMenuItem {
private:
	EventListener<MouseEnterExitFilter> m_listener = {
		this, &DialogButton::onMouseAccessState,
		MouseEnterExitFilter(this, false)
	};

	void onMouseAccessState(MouseMoveType type, CCPoint location);

	void onMouseEnter();
	void onMouseExit();
};

#endif