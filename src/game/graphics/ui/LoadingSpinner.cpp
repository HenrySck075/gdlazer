#include "LoadingSpinner.hpp"
#include "../../../framework/graphics/sprites/FontAwesome.hpp"
#include <henrysck075.easings/include/easings.hpp>

bool LoadingSpinner::init(bool boxed, bool invert) {
    if (!VisibilityContainer::init()) return false;
    
    GLubyte boxColor = invert ? 255 : 0;
    GLubyte spinnerColor = invert ? 0 : 255;
    setColor({255,255,255});
    if (boxed) {
        setColor({boxColor,boxColor,boxColor,255});
        setRadius(20);
    }
    setScale(0.8);
    setAnchorPoint({0.5,0.5});
    setOpacity(255);
    /*    
    spinner = static_cast<CCLabelBMFont*>(FontAwesome::Solid::CircleNotch);
    if (boxed) spinner->setScale(0.6);
    spinner->setColor({spinnerColor,spinnerColor,spinnerColor});
    addListener("nodeLayoutUpdate", [this](NodeEvent* e){
        spinner->setContentSize(CCNode::getContentSize());
        spinner->setPosition(CCNode::getContentSize()/2);
    });
    addChild(spinner);
    */
    log::debug("[LoadingSpinner]: {}", getColor());
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
    );
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
    /*
    spinner->runAction(CCRepeatForever::create(
        CCSequence::createWithTwoActions(
            CCRotateTo::create(spin_duration*3.5,360),
            CCRotateTo::create(0,0)
        )
    ));
    */
}
void LoadingSpinner::onClose() {
    stopAllActions();
    runAction(
        CCSpawn::createWithTwoActions(
            easingsActions::CCEaseIn::create(
                CCScaleTo::create(TRANSITION_DURATION,0.8),2
            ),
            easingsActions::CCEaseOut::create(
                CCFadeOut::create(TRANSITION_DURATION),5
            )
        )
    );
}