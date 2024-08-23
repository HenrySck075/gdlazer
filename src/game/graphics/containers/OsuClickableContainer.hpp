#pragma once

#include <Geode/Geode.hpp>
#include "Geode/utils/MiniFunction.hpp"
#include "../../../framework/input/events/MouseEvent.hpp"
#include "../../../framework/graphics/containers/Container.hpp"

using namespace geode::prelude;
using ButtonCallback = geode::utils::MiniFunction<void(CCNode*)> ;

// button base
class OsuClickableContainer : public Container {
private:
    ButtonCallback clickCallback;
    std::string clickSfx = "";


    bool holding = false;

    //bool ccTouchBegan(CCTouch* t, CCEvent* idc) override;
    //void ccTouchEnded(CCTouch* t, CCEvent* idc) override;

protected:
    void onClick() override {
        Container::onClick();
        FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx);
        this->clickCallback(this);
    };

public:
    // pretend to be clicking
    void click() {dispatchEvent(new MouseEvent(MouseEventType::Click, ccp(9,9)));};
    void setClickSFX(std::string sfx) { clickSfx = sfx; };
    void setCallback(ButtonCallback clickCb) { clickCallback = clickCb; };
    /// <param name="clickCb">| the</param>
    /// <param name="self">| the node to be used as the clickCb parameter</param>
    bool init(std::string clickSfx, ButtonCallback clickCb, CCNode* self = nullptr);
    
    
  /*
    // redirecting functions
    void activate() override {
        if (!m_clickEnabled) return;
        onClick();
        FMODAudioEngine::sharedEngine()->playEffect(clickSfx);
        clickCallback(this);
    };
    void selected() override {
        log::debug("[CCNodeHover]: selected");
        m_bSelected = true;
        onMouseDown();
    };
    void unselected() override {
        log::debug("[CCNodeHover]: unselected");
        m_bSelected = false;
        onMouseUp();
    };
*/
};

