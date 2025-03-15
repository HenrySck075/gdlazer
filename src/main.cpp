#include "frameworks/graphics/containers/FillFlowContainer.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include "frameworks/graphics/containers/Container.hpp"
struct e : public Modify<e, MenuLayer> {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(e::onMyButton)
		);
		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

        return true;
    };
    void onMyButton(CCObject*) {
		auto flowContainer = gdlazer::FillFlowContainer::create(gdlazer::FillDirection::Horizontal);
		for (int i = 0; i < 5; i++) {
			auto us = gdlazer::Container::create();
			us->setSize({120,50});
			us->setBorderRadius(10);
			us->setBackgroundColor({255,255,255,255});
			flowContainer->addChild(us);
		}
		flowContainer->updateLayout();
		this->addChild(flowContainer);
	}
};