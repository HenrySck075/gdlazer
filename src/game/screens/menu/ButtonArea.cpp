#include "ButtonArea.hpp"
#include "../../../helpers/CustomActions.hpp"

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

void ButtonArea::constructButtons(std::vector<MainMenuButton*>& buttons, int index) {
    int _index = index!=-1?index:buttonsMenus.size();
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
    b->setID(std::to_string(_index)+"_2");
    
    auto t = CCLayer::create();
    t->ignoreAnchorPointForPosition(false);
    t->setAnchorPoint(ccp(1,0.5));
    t->setPosition(anchorPosition-gap);
    t->addChild(buttons[0]);
    buttons[0]->setAnchorPoint(ccp(1,0.5));
    t->setLayout(RowLayout::create()->setGap(-1)->setAutoScale(false)->setAxisAlignment(AxisAlignment::End)->setAxisReverse(true));
    t->updateLayout();
    t->setID(std::to_string(_index)+"_1");

    std::tuple<CCLayer*, CCLayer*> j = std::make_tuple(t,b);
    if (index!=-1) {
        buttonsMenus.insert(buttonsMenus.begin()+index, j);
        // why is cpp says it's deleted is it stupid
        _buttons.insert(_buttons.begin()+index,buttons);
    } else {
        buttonsMenus.push_back(j);
        _buttons.push_back(buttons);
    }

    if (buttonsMenus.size() == 1) {show(0);}
}

void ButtonArea::show(int index) {
    if (shownIndex != -1) hide(shownIndex);
    if (shownIndex==index) return;
    
    // expand
    if (shownIndex<index) {
        auto j = _buttons[index];
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
        this->addChild(get<0>(buttonsMenus[index]));
        this->addChild(get<1>(buttonsMenus[index]));
    }
    // collapse
    else {
    }
}

void ButtonArea::hide(int index) {
        for (auto* i : _buttons[index]) {
            auto pos = i->getPosition();
            i->runAction(CCEaseSineOut::create(
                CCSpawn::createWithTwoActions(
                    CCMoveTo::create(0.5, anchorPosition),
                    CCFadeOut::create(0.5)
                )
            ));
        }
}