#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"
#include "deco/Triangles.hpp"

using namespace geode::prelude;

class DialogButton : public CCMenuItem {
private:
	EventListenerProtocol* m_listener;

	float height = 12;
	ccColor3B m_color;

	void onMouseEnter();
	void onMouseExit();
	bool init(const char* label, ccColor3B color);
	void redraw();

public:
	void setContentSize(const CCSize& size);
	static DialogButton* create(const char* label, ccColor3B color);
	void setParent(CCNode* parent);
};

#endif