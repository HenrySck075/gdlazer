#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"

bool MainMenuButton::init(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction) {
	ButtonBase::init(sampleClick.c_str(), clickAction, this);
	auto the = CCLayer::create();
	the->setLayout(ColumnLayout::create()->setGap(5));
	the->addChild(symbol);
	the->addChild(OsuText(text.c_str(), FontType::Regular));
	auto s9 = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
	s9->setID("background");
	s9->setColor(color);
	this->addChild(s9);
	return true;
}

void MainMenuButton::setContentSize(const CCSize& s) {
	CCNode::setContentSize(s);
	this->getChildByID("background")->setContentSize(s);
}
