#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"

bool MainMenuButton::init(std::string text, std::string sampleClick, CCSprite* symbol, Color4 color, ButtonCallback clickAction) {
	ButtonBase::init(sampleClick.c_str(), clickAction, this);
	auto the = CCLayer::create();
	the->setLayout(ColumnLayout::create()->setGap(5));
	the->addChild(symbol);
	the->addChild(OsuText::create(text.c_str(), FontType::Regular));
	auto s9 = CCScale9Sprite::createWithSpriteFrameName("popupdialogbtn.png"_spr);
	s9->setID("background");
	this->addChild(s9);
	return true;
}

void MainMenuButton::setContentSize(const CCSize& s) {
	CCNode::setContentSize(s);
	this->getChildByID("background")->setContentSize(s);
}
