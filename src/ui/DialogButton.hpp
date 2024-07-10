#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"
#include "deco/Triangles.hpp"

using namespace geode::prelude;

class DialogButton : public CCMenuItem {
private:
	/*
	EventListener<MouseEnterExitFilter> m_listener = {
		this, &DialogButton::onMouseAccessState,
		MouseEnterExitFilter(this, false)
	};
	*/

	float height = 12;
	ccColor3B m_color;

	void onMouseAccessState(MouseMoveType, CCPoint);

	void onMouseEnter();
	void onMouseExit();
	bool init(const char* label, ccColor3B color);
	void redraw();
	void setContentSize(const CCSize& size);

public:
	static DialogButton* create(const char* label, ccColor3B color);
	void setParent(CCNode* parent);
};

#endif