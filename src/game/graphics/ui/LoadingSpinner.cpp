#include "LoadingSpinner.hpp"
#include "../../../framework/graphics/sprites/FontAwesome.hpp"
#include <henrysck075.easings/include/easings.hpp>
#include "../../../helpers/CustomActions.hpp"

bool LoadingSpinner::init(bool boxedP, bool invert) {
    if (!VisibilityContainer::init()) return false;
    
    GLubyte boxColor = invert ? 255 : 0;
    GLubyte spinnerColor = invert ? 0 : 255;
    setColor({boxColor,boxColor,boxColor,0});

    boxed = boxedP;
    if (boxed) {
        setColor({boxColor,boxColor,boxColor,255});
        setRadius(20);
    }
    setScale(0.8);
    setAnchorPoint({0.5,0.5});
    //setUserObject("breakhere",CCBool::create(true));
    
    spinner = FontAwesome::Solid::CircleNotch;
    if (boxed) spinner->setScale(0.6);
    spinner->setColor({spinnerColor,spinnerColor,spinnerColor});
    addListener("nodeLayoutUpdate", [this](NodeEvent* e){
        spinner->setPosition(CCNode::getContentSize()/2);
    });
    addChild(spinner);
    setCascadeColorEnabled(false);
    setCascadeOpacityEnabled(false);
    return true;
}   

LoadingSpinner* LoadingSpinner::create(bool boxed, bool invert) {
    create_class(LoadingSpinner, init, boxed, invert);
}

void LoadingSpinner::onOpen() {
    stopAllActions();
    runAction(
        easingsActions::CCEaseOut::create(
            CCSpawn::createWithTwoActions(
                CCScaleTo::create(TRANSITION_DURATION,1),
                CCFadeIn::create(TRANSITION_DURATION)
            ), 5
        )
    )->setTag(7);
    if (boxed) {
        runAction(CCRepeatForever::create(
            CCSequence::create(
                easingsActions::CCEaseInOut::create(
                    CCRotateTo::create(spin_duration,90),4
                ),
                easingsActions::CCEaseInOut::create(
                    CCRotateTo::create(spin_duration,180),4
                ),
                easingsActions::CCEaseInOut::create(
                    CCRotateTo::create(spin_duration,270),4
                ),
                easingsActions::CCEaseInOut::create(
                    CCRotateTo::create(spin_duration,360),4
                ),
                CCRotateTo::create(0,0),
                nullptr
            )
        ));
    }
    log::debug("{}", _displayedOpacity);
    spinner->setOpacity(255);
    spinner->runAction(CCRepeatForever::create(
        CCSequence::createWithTwoActions(
            CCRotateBy::create(spin_duration*3.5,360),
            CCRotateTo::create(0,0)
        )
    ));
}
void LoadingSpinner::onClose() {
    stopActionByTag(7);
    runAction(
        CCSequence::createWithTwoActions(
            CCSpawn::createWithTwoActions(
                easingsActions::CCEaseIn::create(
                    CCScaleTo::create(TRANSITION_DURATION,0.8),2
                ),
                easingsActions::CCEaseOut::create(
                    CCFadeOut::create(TRANSITION_DURATION),5
                )
            ),
            CCCallFuncL::create([this]{stopAllActions();})
        )
    );
}