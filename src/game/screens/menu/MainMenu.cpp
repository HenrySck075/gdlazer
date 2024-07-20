#include "MainMenu.hpp"
#include "OsuLogo.hpp"

bool MainMenu::init(bool flash) {
	auto logo = OsuLogo::create();
	logo->setPosition(CCDirector::sharedDirector()->getWinSize() / 2);
	return true;
}