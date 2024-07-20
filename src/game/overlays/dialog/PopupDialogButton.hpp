#pragma once

#include <Geode/Geode.hpp>
#include "../../graphics/ui/ButtonBase.hpp"

using namespace geode::prelude;

class PopupDialogButton : public ButtonBase {
private:

	float height = 16;
	ccColor3B m_color;
	const char* m_sfx;

	void onMouseEnter() override;
	void onMouseExit() override;
	bool init(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);

public:
	void setOpacity(GLubyte opacity) override;
	void setContentSize(const CCSize& size) override;
	static PopupDialogButton* create(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb);
	void setContentHeight(float height);
	void setParent(CCNode* parent) override;
};

