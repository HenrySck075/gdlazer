#include "LoadingSpinner.hpp"
#include "../../../frameworks/graphics/sprites/FontAwesome.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"
#include "../../../helpers/CustomActions.hpp"

GDL_NS_START
using namespace frameworks;
bool LoadingSpinner::init(bool boxedP, bool invert) {
  if (!VisibilityContainer::init()) return false;
  
  GLubyte boxColor = invert ? 255 : 0;
  GLubyte spinnerColor = invert ? 0 : 255;
  setBackgroundColor({boxColor,boxColor,boxColor,0});

  boxed = boxedP;
  if (boxed) {
    setBackgroundColor({boxColor,boxColor,boxColor,255});
    setBorderRadius(20);
  }
  setScale(0.8);
  setAnchorPoint({0.5,0.5});
  //setUserObject("breakhere",CCBool::create(true));
  
  spinner = FontAwesome::Solid::CircleNotch;
  if (boxed) spinner->setScale(0.6);
  spinner->setColor({spinnerColor,spinnerColor,spinnerColor});
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated*){
    spinner->setContentSize(CCNode::getContentSize());
    spinner->setPosition(CCNode::getContentSize()/2);
    return true;
  });
  addChild(spinner);
  setCascadeOpacityEnabled(false);
  return true;
}   

LoadingSpinner* LoadingSpinner::create(bool boxed, bool invert) {
  $createClass(LoadingSpinner, init, boxed, invert);
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
GDL_NS_END
