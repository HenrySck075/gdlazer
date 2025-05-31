#include "utils.hpp"
#include <type_traits>

float randomFloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

bool randomBool() {
  return randomFloat() >= 0.5;
}


// get the original content size since Container overrides it
CCSize getRealScaledContentSize(CCNode* node) {
  auto sizeUnscaled = node->CCNode::getContentSize(); 
  return CCSize{sizeUnscaled.width*node->getScaleX(), sizeUnscaled.height*node->getScaleY()};
}
/// <summary>
/// im suck at naming so uhm read the code
/// </summary>
/// <param name="node"></param>
/// <returns>the node's bottom-left position</returns>
CCPoint getPositionAnchorAware(CCNode* node) {
  auto pos = node->CCNode::getPosition();
  auto size = getRealScaledContentSize(node);
  auto anchor = node->isIgnoreAnchorPointForPosition() ? CCPoint{0,0} : node->getAnchorPoint();

  return CCPoint{ pos.x - size.width * anchor.x,pos.y - size.height * anchor.y };
}

CCRect boundingBoxFromContentSize(CCNode* node) {
  auto size = getRealScaledContentSize(node);

  auto pos = getPositionAnchorAware(node);
  auto parent = node->getParent();
  while (parent != nullptr) {
    pos += getPositionAnchorAware(parent);
    //log::debug("[boundingBoxFromContentSize]:n pos: {} | parent: {}", pos, parent);
    parent = parent->getParent();
  }

  return CCRect{ pos.x, pos.y, size.width, size.height };
}

CCPoint convertToNodeSpaceA(CCNode* node, CCPoint point) {
  auto box = boundingBoxFromContentSize(node);
  //geode::log::debug("[ctnsa]: {} | {}", box.origin, point);
  return point - box.origin;
};

CCRect flipRect(CCRect rect) {
  auto ws = CCDirector::sharedDirector()->getWinSize();
  return CCRect{ rect.getMinX(),ws.height - rect.getMinY() + rect.size.height,rect.size.width, rect.size.height };
}


#include <algorithm> 
#include <cctype>
#include <locale>

std::string& trim( std::string& s, char c, bool reverse )
{
  return reverse
  ? s.erase( s.find_last_not_of( c ) + 1 )
  : s.erase( 0, s.find_first_not_of( c ) );
}

template<typename TargetType, typename ComparingType>
bool isType(ComparingType obj) {
  return std::is_same_v<TargetType, ComparingType>;
};

float uiWidthToGL(float length) {
  auto d = CCDirector::sharedDirector();
  return length / d->getOpenGLView()->getFrameSize().width * d->getWinSize().width;
}
float uiHeightToGL(float length) {
  auto d = CCDirector::sharedDirector();
  return length / d->getOpenGLView()->getFrameSize().height * d->getWinSize().height;
}

#ifndef GEODE_IS_WINDOWS
#include <cxxabi.h>
#endif
std::string getObjectName(CCObject* node){
#ifdef GEODE_IS_WINDOWS
  return typeid(*node).name() + 6;
#else 
  {
    std::string ret;

    int status = 0;
    auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
    if (status == 0) {
      ret = demangle;
    }
    free(demangle);

    return ret;
  }
#endif
}

uintptr_t getGeodeLib() {
#ifdef GEODE_IS_WINDOWS
  static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA("Geode.dll"));
  return base;
#elif GEODE_IS_ANDROID
  static std::uintptr_t basePtr = 0u;
  if (basePtr == 0u) {
      auto handle = getLibHandle();

      // JNI_OnLoad is present on all versions of GD
      auto sym = dlsym(handle, "JNI_OnLoad");
      assert(sym != nullptr);

      Dl_info p;
      auto dlAddrRes = dladdr(sym, &p);
      assert(dlAddrRes != 0);

      basePtr = reinterpret_cast<std::uintptr_t>(p.dli_fbase);
  }

  return basePtr;
#else
  return 0;
#endif
}