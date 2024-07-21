#pragma once

#include <Geode/Geode.hpp>
//#include "ButtonSystem.hpp"
#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;
using MenuSideFlashes = CCLayer*;

class MainMenu : public CCLayer {
public:
	float FADE_IN_DURATION = 300;

	float FADE_OUT_DURATION = 400;

	bool hideOverlaysOnEnter = false;//buttons == nullptr;// || Buttons.State == ButtonSystemState.Initial;

	static MainMenu* create(bool flash) {
		create_class(MainMenu, init, flash);
	}
	bool init(bool flash);
private:
	BeatDetector* detector;

	CCLayer* songSelect;

	MenuSideFlashes sideFlashes;

protected:
	int idkTho = 0;
  //ButtonSystem buttons;
};
