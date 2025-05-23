#include "CCClippingNodeRGBA.hpp"

void CCClippingNodeRGBA::setOpacity(GLubyte opacity) {
  _displayedOpacity = _realOpacity = opacity;
    
	if (_cascadeOpacityEnabled) {
		GLubyte parentOpacity = 255;
      CCRGBAProtocol* pParent = geode::cast::typeinfo_cast<CCRGBAProtocol*>(m_pParent);
      if (pParent && pParent->isCascadeOpacityEnabled()) {
        parentOpacity = pParent->getDisplayedOpacity();
      }
    this->updateDisplayedOpacity(parentOpacity);
	}
}

GLubyte CCClippingNodeRGBA::getOpacity() { return _realOpacity; }
void CCClippingNodeRGBA::setColor(const cocos2d::ccColor3B &color) {
  _realColor = color;
  if (_cascadeColorEnabled) {
    cocos2d::ccColor3B parentColor = cocos2d::ccWHITE;
    if (auto parent = geode::cast::typeinfo_cast<CCRGBAProtocol *>(getParent())) {
      parentColor = parent->getDisplayedColor();
    }
    updateDisplayedColor(parentColor);
  } else {
    _displayedColor = _realColor;
  }
}
const cocos2d::ccColor3B &CCClippingNodeRGBA::getColor() { return _realColor; }
const cocos2d::ccColor3B &CCClippingNodeRGBA::getDisplayedColor() {
  return _displayedColor;
}
GLubyte CCClippingNodeRGBA::getDisplayedOpacity() { return _displayedOpacity; }
void CCClippingNodeRGBA::updateDisplayedOpacity(GLubyte parentOpacity) {
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
	
  if (_cascadeOpacityEnabled) {
    CCObject* pObj;
    CCARRAY_FOREACH(m_pChildren, pObj) {
      CCRGBAProtocol* item = geode::cast::typeinfo_cast<CCRGBAProtocol*>(pObj);
      if (item) {
        item->updateDisplayedOpacity(_displayedOpacity);
      }
    }
  }
}

void CCClippingNodeRGBA::updateDisplayedColor(
    const cocos2d::ccColor3B &parentColor) {
  _displayedColor.r = _realColor.r * parentColor.r / 255;
  _displayedColor.g = _realColor.g * parentColor.g / 255;
  _displayedColor.b = _realColor.b * parentColor.b / 255;
  if (_cascadeColorEnabled) {
    for (auto child : geode::cocos::CCArrayExt<CCNode>(getChildren())) {
      if (auto rgbaChild = geode::cast::typeinfo_cast<CCRGBAProtocol *>(child)) {
        rgbaChild->updateDisplayedColor(_displayedColor);
      }
    }
  }
}
bool CCClippingNodeRGBA::isCascadeOpacityEnabled() {
  return _cascadeOpacityEnabled;
}
void CCClippingNodeRGBA::setCascadeOpacityEnabled(bool cascadeOpacityEnabled) {
  _cascadeOpacityEnabled = cascadeOpacityEnabled;
}
bool CCClippingNodeRGBA::isCascadeColorEnabled() {
  return _cascadeColorEnabled;
}
void CCClippingNodeRGBA::setCascadeColorEnabled(bool cascadeColorEnabled) {
  _cascadeColorEnabled = cascadeColorEnabled;
}
