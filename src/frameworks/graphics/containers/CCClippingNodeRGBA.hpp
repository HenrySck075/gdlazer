#include <Geode/cocos/misc_nodes/CCClippingNode.h>

using namespace cocos2d;

class CCClippingNodeRGBA : public cocos2d::CCClippingNode, public cocos2d::CCRGBAProtocol {
protected:
  GLubyte _displayedOpacity;
  GLubyte _realOpacity;
  cocos2d::ccColor3B _displayedColor;
  cocos2d::ccColor3B _realColor;
  bool _cascadeOpacityEnabled;
  bool _cascadeColorEnabled;

public:
  CCClippingNodeRGBA()
    : _displayedOpacity(255),
      _realOpacity(255),
      _displayedColor(cocos2d::ccWHITE),
      _realColor(cocos2d::ccWHITE),
      _cascadeOpacityEnabled(false),
      _cascadeColorEnabled(false) {}

  static CCClippingNodeRGBA* create() {
    CCClippingNodeRGBA* ret = new CCClippingNodeRGBA();
    if (ret && ret->init()) {
      ret->autorelease();
      return ret;
    }
    delete ret;
    return nullptr;
  }

  // CCRGBAProtocol methods
  GLubyte getOpacity() override;
  GLubyte getDisplayedOpacity() override;
  void setOpacity(GLubyte opacity) override;
  void updateDisplayedOpacity(GLubyte parentOpacity) override;
  bool isCascadeOpacityEnabled() override;
  void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override;

  const cocos2d::ccColor3B& getColor() override;
  const cocos2d::ccColor3B& getDisplayedColor() override;
  void setColor(const cocos2d::ccColor3B& color) override;
  void updateDisplayedColor(const cocos2d::ccColor3B &parentColor) override;
  bool isCascadeColorEnabled() override;
  void setCascadeColorEnabled(bool cascadeColorEnabled) override;

  void setOpacityModifyRGB(bool bValue) override {}
  bool isOpacityModifyRGB() override {return false;}
};