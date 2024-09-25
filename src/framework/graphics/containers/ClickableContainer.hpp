/// @note Moved from osu!game because fym click sfx is only available in OsuClickableContainer

#pragma once

#include <Geode/Geode.hpp>
#include "Geode/utils/MiniFunction.hpp"
#include "../../input/events/MouseEvent.hpp"
#include "Container.hpp"

using namespace geode::prelude;
using ButtonCallback = geode::utils::MiniFunction<void(CCNode*)> ;

// e
class ClickableContainer : public Container {
private:
    ButtonCallback clickCallback;
    std::string clickSfx = "";


    bool holding = false;

    //bool ccTouchBegan(CCTouch* t, CCEvent* idc) override;
    //void ccTouchEnded(CCTouch* t, CCEvent* idc) override;

protected:
    void onClick(MouseEvent* e) override {
        Container::onClick(e);
        FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx);
        
        if (clickCallback) clickCallback(this);
    };

public:
    // pretend to be clicking
    void click() {dispatchEvent(new MouseEvent(MouseEventType::Click, ccp(9,9)));};
    void setClickSFX(std::string sfx) { clickSfx = sfx; };
    void setCallback(ButtonCallback clickCb) { clickCallback = clickCb; };
    /// <param name="clickCb">| the</param>
    /// <param name="self">| the node to be used as the clickCb parameter</param>
    bool init(std::string clickSfx, ButtonCallback clickCb);
    bool init(std::string clickSfx);
    
    
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

