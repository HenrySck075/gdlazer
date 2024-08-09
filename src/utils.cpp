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
    auto pos = node->getPosition();
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
  return length / d->getOpenGLView()->getDisplaySize().width * d->getWinSize().width;
}
float uiHeightToGL(float length) {
  auto d = CCDirector::sharedDirector();
  return length / d->getOpenGLView()->getDisplaySize().height * d->getWinSize().height;
}