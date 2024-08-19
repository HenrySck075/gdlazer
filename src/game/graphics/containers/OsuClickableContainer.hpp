#pragma once

#include <Geode/Geode.hpp>
#include "Geode/utils/MiniFunction.hpp"
#include "../../MouseEvent.hpp"
#include "../../../framework/graphics/containers/Container.hpp"

using namespace geode::prelude;
using ButtonCallback = geode::utils::MiniFunction<void(CCNode*)> ;

// button base
class OsuClickableContainer : public Container {
private:
    ButtonCallback clickCallback;
    std::string clickSfx = "";
    bool m_clickEnabled = true;
    bool m_hoverEnabled = true;
    bool m_entered = false;
    bool m_holding = false;
    EventListenerProtocol* m_listener;

    bool holding = false;

    //bool ccTouchBegan(CCTouch* t, CCEvent* idc) override;
    //void ccTouchEnded(CCTouch* t, CCEvent* idc) override;

protected:
    virtual void onClick() = 0;
    virtual void onMouseDown() = 0;
    virtual void onMouseUp() = 0;
    virtual void onMouseEnter() = 0;
    virtual void onMouseExit() = 0;

public:
    void setClickSFX(std::string sfx) { clickSfx = sfx; };
    void setCallback(ButtonCallback clickCb) { clickCallback = clickCb; };
    /// <param name="clickCb">| the</param>
    /// <param name="self">| the node to be used as the clickCb parameter</param>
    bool init(std::string clickSfx, ButtonCallback clickCb, CCNode* self = nullptr);
    void setClickEnabled(bool e) {m_clickEnabled = e;}
    bool getClickEnabled() {return m_clickEnabled;}
    void setHoverEnabled(bool state) { 
        m_hoverEnabled = state; 
        if (!m_hoverEnabled) {onMouseExit();}
    else {
#ifdef GEODE_IS_WINDOWS
        auto director = CCDirector::sharedDirector();
        auto pos = director->getOpenGLView()->getMousePosition();
        auto realSize = director->getOpenGLView()->getDisplaySize();
        auto winSize = director->getWinSize();

        auto p = CCPoint(
            pos.x / realSize.width * winSize.width, 
            ((realSize.height-pos.y) / realSize.height * winSize.height)
        );

        dispatchEvent(new MouseEvent(MouseEventType::Move, p));
#endif
    }
    };
    bool getHoverEnabled() { return m_hoverEnabled; }
    
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

