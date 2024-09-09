#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"
#include "../../../framework/input/events/KeyEvent.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"

bool MainMenuButton::init(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAct, std::vector<enumKeyCodes> activa) {
    auto m = CCSize(BUTTON_WIDTH,BUTTON_AREA_HEIGHT);
    m_color = color;
    OsuClickableContainer::init(sampleClick, clickAct);
    OsuClickableContainer::setContentSize(m);
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
    symbol->setScale(0.62);
    the->setLayout(ColumnLayout::create()->setGap(3)->setAutoScale(false)->setAxisReverse(true));
    the->addChild(symbol);
    the->setAnchorPoint(ccp(0.5,0.5));
    the->setPosition(m/2);
    auto label = OsuText(text.c_str(), FontType::Regular);
    label->setScale(0.4);
    the->addChild(label);
    the->updateLayout();
    
    #define bgSetup(var, id, col)                                           \
    auto var = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr); \
    var->setID(id);                                                         \
    var->setColor(col);                                                     \
    var->setSkewX(7);                                                       \
    var->setContentSize(m);                                                 \
    var->setAnchorPoint(CCPoint(0.5,0.5));                                  \
    var->setPosition(m/2)

    bgSetup(s9, "background", color);
    bgSetup(s9hover, "hover", ccc3(255,255,255));
    s9hover->setUserObject("opacityCascadeBlacklist", CCBool::create(true));
    s9hover->setOpacity(0);
    this->addChild(s9);
    this->addChild(s9hover);
    this->addChild(the);
    
    this->setAnchorPoint(ccp(0.5,0.5));
    this->setCascadeOpacityEnabled(true);
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

// keep being invisible, but cascade opacity to the child nodes
void MainMenuButton::updateDisplayedOpacity(GLubyte parentOpacity) {
    
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
    
    if (_cascadeOpacityEnabled) {
        CCObject *obj = NULL;
        CCARRAY_FOREACH(m_pChildren, obj) {
            CCRGBAProtocol *item = typeinfo_cast<CCRGBAProtocol*>(obj);
            if (item) {
                CCBool* _b = static_cast<CCBool*>(typeinfo_cast<CCNode*>(item)->getUserObject("opacityCascadeBlacklist"));
                    
                if (!(_b!=nullptr && _b->getValue())) item->updateDisplayedOpacity(_displayedOpacity);
            }
        }
    }
    _displayedOpacity = 0;
}

void MainMenuButton::setContentSize(const CCSize& size) {
    OsuClickableContainer::setContentSize(size);
    auto s = getContentSize();
    // todo: move to container
    if (auto bg = this->getChildByID("background")) {
        bg->setContentSize(s);
        bg->setPosition(s/2);
    }
    if (auto bgh = this->getChildByID("hover")) {
        bgh->setContentSize(s);
        this->getChildByID("hover")->setPosition(s/2);
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
    n->runAction(CCEaseOutQuint::create(
        CCFadeTo::create(1,255*0.1)
    ));
}
void MainMenuButton::onMouseUp(MouseEvent* event) {
    //if (!static_cast<CCBool*>(this->getUserObject("clicking"_spr))) return;
    auto n = static_cast<CCScale9Sprite*>(this->getChildByID("hover"));
    n->runAction(CCEaseOutQuint::create(
        CCFadeTo::create(1,0)
    ));
}
void MainMenuButton::onClick(MouseEvent* event) {
    OsuClickableContainer::onClick(event);
    auto node = static_cast<CCScale9Sprite*>(this->getChildByID("hover"));
    node->stopAllActions();
    node->setOpacity(255*0.9);
    node->runAction(CCEaseExponentialOut::create(
        CCFadeTo::create(0.8,0)
    ));
}
