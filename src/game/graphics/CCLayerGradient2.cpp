#include "CCLayerGradient2.hpp"

bool CCLayerGradient2::initWithTarget(const ccColor4B& start, const ccColor4B& end, const CCPoint& v, CCLG2Target targetUpdate) {
  if (!initWithColor(start,end,v)) return false;
  setTargetColorUpdate(targetUpdate);
  setTargetOpacityUpdate(targetUpdate);
  return true;
};

bool CCLayerGradient2::initWithTarget(const ccColor4B& start, const ccColor4B& end, CCLG2Target targetUpdate) {
  return initWithTarget(start,end,{0,0},targetUpdate);
}