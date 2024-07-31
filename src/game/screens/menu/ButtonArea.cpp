#include "ButtonArea.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "Geode/loader/Log.hpp"
#include <Geode/utils/ranges.hpp>

bool ButtonArea::init(const CCPoint& anchorPos) {
    anchorPosition = anchorPos;
    ignoreAnchorPointForPosition(false); 
    setAnchorPoint(ccp(0.5,0.5));
    return true;
}

void ButtonArea::setParent(CCNode* parent) {
    CCLayer::setParent(parent);
    this->setContentSize(parent->getContentSize());
    this->setPosition(parent->getContentSize()/2);
}

void ButtonArea::constructButtons(std::vector<MainMenuButton*> buttons, std::string tag) {
    auto b = CCLayer::create();
    auto gap = ccp(20,0);
    b->ignoreAnchorPointForPosition(true);
    b->setAnchorPoint(ccp(0,0.5));
    b->setPosition(anchorPosition+gap);
    b->setLayout(RowLayout::create()->setGap(-1)->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setAxisReverse(true));
    int buttonsCount = buttons.size();
    for (int i = 1; i<buttonsCount; i++) {
        b->addChild(buttons[i]);
        buttons[i]->setZOrder(buttonsCount-i);
    };
    b->updateLayout();
    b->setID(tag+"_2");
    
    auto t = CCLayer::create();
    t->ignoreAnchorPointForPosition(false);
    t->setAnchorPoint(ccp(1,0.5));
    t->setPosition(anchorPosition-gap);
    t->addChild(buttons[0]);
    buttons[0]->setAnchorPoint(ccp(1,0.5));
    t->setLayout(RowLayout::create()->setGap(-1)->setAutoScale(false)->setAxisAlignment(AxisAlignment::End)->setAxisReverse(true));
    t->updateLayout();
    t->setID(tag+"_1");

    std::tuple<CCLayer*, CCLayer*> j = std::make_tuple(t,b);
        buttonsMenus[tag] = j;
        _buttons[tag] = buttons;

    if (buttonsMenus.size() == 1) {show(tag);}
}

void ButtonArea::show(std::string tag) {
    auto _cur = getCurrent();
    // just for comparison
    auto shownIndex = ranges::indexOf(tagsStack, [_cur](std::string t){return t==_cur;}).value_or(-1);
    auto index = ranges::indexOf(tagsStack, [tag](std::string t){return t==tag;}).value_or(-1);
    if (index==-1) {
        if (buttonsMenus.find(tag)==buttonsMenus.end()) {
            log::error("[ButtonArea]: \"{}\" is not a registered menu.", tag);
            return;
        }
        index = tagsStack.size();
        tagsStack.push_back(tag);
    }
    if (shownIndex != -1) hide(_cur);
    if (shownIndex==index) return;
    
    // expand
    auto j = _buttons[tag];
    if (shownIndex<index) {
        for (int idx = 0; idx<j.size(); idx++) {
            auto i = j[idx];
            auto pos = i->getPosition();
            if (idx!=0) {i->setPositionX(-15);}
            else {i->setPositionX(pos.x+15);}
            i->runAction(CCSequence::createWithTwoActions(
                CCEaseSineOut::create(CCSpawn::createWithTwoActions(
                    CCMoveTo::create(0.25, pos),
                    CCFadeIn::create(0.25)
                )),
                CCCallFuncL::create([i](){i->setHoverEnabled(true);})
            ));
        }
        this->addChild(get<0>(buttonsMenus[tag]));
        this->addChild(get<1>(buttonsMenus[tag]));
    }
    // return
    else {
        for (int idx = 0; idx<j.size(); idx++) {
            auto i = j[idx];
            auto pos = i->getPosition();
            if (idx!=0) {i->setPositionX(-15);}
            else {i->setPositionX(pos.x+15);}
            i->runAction(CCSequence::createWithTwoActions(
                CCEaseSineOut::create(CCSpawn::createWithTwoActions(
                    CCMoveTo::create(0.25, pos),
                    CCFadeIn::create(0.25)
                )),
                CCCallFuncL::create([i](){i->setHoverEnabled(true);})
            ));
        }
        // the node is still there so we dont need to add them
        // however
        while (tagsStack.back()!=tag) {tagsStack.pop_back();}
    }
}

void ButtonArea::hide(std::string tag) {
    for (auto* i : _buttons[tag]) {
        auto pos = i->getPosition();
        i->runAction(CCEaseSineOut::create(
            CCSpawn::createWithTwoActions(
                CCMoveTo::create(0.5, anchorPosition),
                CCFadeOut::create(0.5)
            )
        ));
    }
}
