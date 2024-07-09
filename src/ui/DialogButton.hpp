#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"
#include "deco/Triangles.hpp"

using namespace geode::prelude;

class DialogButton : public CCMenuItem {
private:
	float height = 12;
	ccColor3B m_color;

	EventListener<MouseEnterExitFilter> m_listener = {
		this, &DialogButton::onMouseAccessState,
		MouseEnterExitFilter(this, false)
	};

	void onMouseAccessState(MouseMoveType type, CCPoint location);

	void onMouseEnter();
	void onMouseExit();
	bool init(std::string& label, ccColor3B color);
	void redraw();
	void setContentSize(const CCSize& size);

public:
	static DialogButton* create(std::string& label, ccColor3B color);
	void setParent(CCNode* parent);
};

#endif