#include <Geode/cocos/misc_nodes/CCClippingNode.h>
#include "../../../utils.hpp"

using namespace cocos2d;

class CCClippingNodeRGBA : public cocos2d::CCClippingNode, public cocos2d::CCRGBAProtocol {
protected:
  GLubyte _displayedOpacity = 255;
  GLubyte _realOpacity = 255;
  cocos2d::ccColor3B _displayedColor {255,255,255};
  cocos2d::ccColor3B _realColor {255,255,255};
  bool _cascadeOpacityEnabled = false;
  bool _cascadeColorEnabled = false;

public:
  static CCClippingNodeRGBA* create(cocos2d::CCNode* stencil) {
    $createClass(CCClippingNodeRGBA, init, stencil);
  }
  static CCClippingNodeRGBA* create() {
    $createClass(CCClippingNodeRGBA, init);
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
