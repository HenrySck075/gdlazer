#ifndef __osu_ui_deco_rectangle__
#define __osu_ui_deco_rectangle__

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class CCRectangle : public CCNodeRGBA {
private:
	ccColor4F m_color;
	void redraw();
public:
	void setContentSize(const CCSize& size);
	void setOpacity(GLubyte opacity);
	static CCRectangle* create(ccColor4F color);
	bool init(ccColor4F color);
};

#endif