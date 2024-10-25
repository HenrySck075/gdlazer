#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"
#include "../../../framework/input/events/KeyEvent.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"
#include <henrysck075.easings/include/easings.hpp>

bool MainMenuButton::init(std::string text, std::string sampleClick, IconConstructor symbol, Color4 color, ButtonCallback clickAct, std::vector<enumKeyCodes> activa) {
    auto m = CCSize(BUTTON_WIDTH,BUTTON_AREA_HEIGHT);
    m_color = color;
    ClickableContainer::init(sampleClick, clickAct);
    ClickableContainer::setContentSize(m);
    //this->addListener(reactive_listener(updateReactive));

    std::transform(text.begin(), text.end(), text.begin(), [](char in) -> char {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    });
    this->activationKeys = activa;
    auto the = CCLayerRGBA::create();
    the->setCascadeOpacityEnabled(true);
    the->setID("ui");
    CCResizableSprite* icon = symbol;
    icon->setScale(0.62);
    the->setLayout(ColumnLayout::create()->setGap(3)->setAutoScale(false)->setAxisReverse(true));
    the->addChild(icon);
    the->setAnchorPoint({0.5,0.5});
    the->setPosition(m/2);
    auto label = OsuText(text.c_str(), FontType::Regular);
    label->setScale(0.4);
    the->addChild(label);
    the->updateLayout();
    
    #define bgSetup(var, id, col)                                           \
    var = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr); \
    var->setID(id);                                                         \
    var->setColor(col);                                                     \
    var->setSkewX(7);                                                       \
    var->setContentSize(m);                                                 \
    var->setAnchorPoint(CCPoint(0.5,0.5));                                  \
    var->setPosition(m/2);                                                  \

    bgSetup(background, "background", color);
    bgSetup(hover, "hover", ccc3(255,255,255));
    this->setCascadeOpacityEnabled(true);
    setColor(Color4{255,255,255,0});
    this->addChild(background);
    this->addChild(hover);
    this->addChild(the);
    hover->setOpacity(0);
    hover->setOpacity(0);
    hover->setOpacity(0);
    hover->setOpacity(0);
    hover->setOpacity(0);
    
    this->setAnchorPoint({0.5,0.5});
    setHoverEnabled(false);
    setClickEnabled(false);

    addListener("keyboardEvent", [this](NodeEvent* ev){
        auto e = static_cast<KeyboardEvent*>(ev);
        if (
            e->key.pressed &&
            std::find(activationKeys.begin(), activationKeys.end(), e->key.key) != activationKeys.end()
        ) {
            click();
        }
    });
    return true;
}

void MainMenuButton::setContentSize(const CCSize& size) {
    ClickableContainer::setContentSize(size);
    auto s = getContentSize();
    // todo: move to container
    if (background) {
        background->setContentSize(s);
        background->setPosition(s/2);
    }
    if (hover) {
        hover->setContentSize(s);
        hover->setPosition(s/2);
    }
    if (auto ui = this->getChildByID("ui")) ui->setPosition(s/2);
    if (m_askForUpdate) m_pParent->updateLayout();
}

void MainMenuButton::onMouseEnter() {
    this->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH*1.5,BUTTON_AREA_HEIGHT)
    ));
    FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav"_spr);
}
void MainMenuButton::onMouseExit() {
    this->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH,BUTTON_AREA_HEIGHT)
    ));
}
void MainMenuButton::onMouseDown(MouseEvent* event) {
    auto n = static_cast<CCScale9Sprite*>(this->getChildByID("hover"));
    n->runAction(easingsActions::CCEaseOut::create(
        CCFadeTo::create(1,255*0.1),5
    ));
}
void MainMenuButton::onMouseUp(MouseEvent* event) {
    //if (!static_cast<CCBool*>(this->getUserObject("clicking"_spr))) return;
    auto n = static_cast<CCScale9Sprite*>(this->getChildByID("hover"));
    n->runAction(easingsActions::CCEaseOut::create(
        CCFadeTo::create(1,0),5
    ));
}
void MainMenuButton::onClick(MouseEvent* event) {
    ClickableContainer::onClick(event);
    auto node = static_cast<CCScale9Sprite*>(this->getChildByID("hover"));
    node->stopAllActions();
    node->setOpacity(255*0.9);
    node->runAction(CCEaseExponentialOut::create(
        CCFadeTo::create(0.8,0)
    ));
}
