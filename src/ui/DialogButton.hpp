#ifndef __osu_ui_dialog_button__
#define __osu_ui_dialog_button__

#include "Geode/Geode.hpp"
#include "../helpers/MouseEvent.hpp"
#include "deco/Triangles.hpp"
#include "deco/Rectangle.hpp"

using namespace geode::prelude;

class DialogButton : public CCNode {
private:
	EventListenerProtocol* m_listener;

	float height = 12;
	ccColor3B m_color;
	const char* m_sfx;

	void onMouseEnter();
	void onMouseExit();
	bool init(const char* label, ccColor3B color, const char* clickSfx);

public:
	void setContentSize(const CCSize& size);
	static DialogButton* create(const char* label, ccColor3B color, const char* clickSfx);
	void setContentHeight(float height);
	void setParent(CCNode* parent);
};

#endif