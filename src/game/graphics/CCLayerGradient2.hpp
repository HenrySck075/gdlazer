#pragma once

#include <Geode/Geode.hpp>
#include "../../utils.hpp"
using namespace cocos2d;

enum class CCLG2Target {Start, End, Both};
/// @brief CCLayerGradient2 is a modification of CCLayerGradient that allows `setColor` and `setOpacity` to target either start or end color.
/// 
/// Implemented as a way to support CCTintTo and CCFadeTo, which calls `setColor` and `setOpacity`, respectively, to do its job
/// 
/// @warning felis catus
class CCLayerGradient2 : public CCLayerGradient {
  CCLG2Target m_targetColorUpdate;
  CCLG2Target m_targetOpacityUpdate;
public:
  // very misleading names
  /** Initializes the CCLayer with a gradient between start and end. 
   *  @js init
   */
  virtual bool initWithTarget(const ccColor4B& start, const ccColor4B& end, CCLG2Target targetUpdate);

  /** Initializes the CCLayer with a gradient between start and end in the direction of v. 
   *  @js init
   */
  virtual bool initWithTarget(const ccColor4B& start, const ccColor4B& end, const CCPoint& v, CCLG2Target targetUpdate);


  /** Creates a full-screen CCLayer with a gradient between start and end. */
  static CCLayerGradient2* create(const ccColor4B& start, const ccColor4B& end, CCLG2Target targetUpdate = CCLG2Target::Both) {
    $create_class(CCLayerGradient2, initWithTarget, start, end, targetUpdate);
  };

  /** Creates a full-screen CCLayer with a gradient between start and end in the direction of v. */
  static CCLayerGradient2* create(const ccColor4B& start, const ccColor4B& end, const CCPoint& v, CCLG2Target targetUpdate = CCLG2Target::Both) {
    $create_class(CCLayerGradient2, initWithTarget, start, end, v, targetUpdate);
  };

  void setTargetColorUpdate(CCLG2Target target) {m_targetColorUpdate = target;}
  void setTargetOpacityUpdate(CCLG2Target target) {m_targetOpacityUpdate = target;}

  void setColor(ccColor3B const& color) override {
    if (m_targetColorUpdate == CCLG2Target::Both) {
      setStartColor(color);
      setEndColor(color);
      return;
    }
    if (m_targetColorUpdate == CCLG2Target::Start) setStartColor(color);
    else setEndColor(color);
  }
  void setOpacity(GLubyte opacity) override {
    if (m_targetOpacityUpdate == CCLG2Target::Both) {
      setStartOpacity(opacity);
      setEndOpacity(opacity);
      return;
    }
    if (m_targetOpacityUpdate == CCLG2Target::Start) setStartOpacity(opacity);
    else setEndOpacity(opacity);
  }
  GLubyte getOpacity() override {
    if (m_targetOpacityUpdate == CCLG2Target::Start) return getStartOpacity();
    else return getEndOpacity();
  }
  const ccColor3B& getColor() override {
    if (m_targetColorUpdate == CCLG2Target::Start) return getStartColor();
    else return getEndColor();
  }
};