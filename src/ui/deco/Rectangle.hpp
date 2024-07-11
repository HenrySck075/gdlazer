#ifndef __osu_ui_deco_rectangle__
#define __osu_ui_deco_rectangle__

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class CCRectangle : public CCNode {
private:
	ccColor4F m_color;
public:
	void setContentSize(const CCSize& size);
	static CCRectangle* create(ccColor4F color);
	bool init(ccColor4F color);
};

#endif