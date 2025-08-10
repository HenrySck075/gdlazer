#pragma once

class ElementBase {

}; 


/// implement the following methods:
/// - cocos2d::CCNode* build();
template<class T>
class Element;

#define $defineElement(cls) \
template<> \
class Element<cls> : public ElementBase
