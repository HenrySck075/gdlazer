#include "FillFlowContainer.hpp"

// axislayout rewrite
//
// dang
class FillFlowLayout : public ContainerLayout {
public:
  bool init() {return true;}
  static FillFlowLayout* create() {
    $create_class(FillFlowLayout, init);
  }
  CCSize getSizeHint(CCNode* on) const override {return on->getContentSize();}
  void apply(CCNode* on) {
    CCArrayExt<Container*> nodes = getNodesToPosition(on);

    auto node = dynamic_cast<FillFlowContainer*>(on);
    // set node size to max constraint size
    auto constraint = node->getSizeConstraints().second;
    auto cwidthOpengl = node->Container::processUnit(constraint.width, Unit::UIKit, true);
    auto cheightOpengl = node->Container::processUnit(constraint.height, Unit::UIKit, false);
    if (constraint.width>0)
      node->CCNode::setContentSize({
        cwidthOpengl, node->CCNode::getContentSize().height
      });
    if (constraint.height>0)
      node->CCNode::setContentSize({
        node->CCNode::getContentSize().width, cheightOpengl
      });
    assert(("l bozo", node==nullptr));

    CCSize size = {0,0};
    breakpoint();
    bool isVertical = node->getFillDirection() == FillDirection::Vertical;
    bool isHorizontal = node->getFillDirection() == FillDirection::Horizontal;
    for (auto c : nodes) {
      //log::debug("[FillFlowLayout]: {}",size.height);
      if (
        // vertical
        (isVertical && c->getSizeUnit().second == Unit::Percent && constraint.height == 0) ||
        // horizontal
        (isHorizontal && c->getSizeUnit().first == Unit::Percent && constraint.width == 0)
      ) {
        throw std::invalid_argument(fmt::format(
          "[FillFlowContainer/Layout]: Child {} has the size dependent "
          "on the parent, but the parent does not have a maximum size constraint. "
          "Please set the maximum constraint to non-zero.", 
          geode::format_as(c)
        ));
      }
      c->setAnchor(Anchor::TopLeft);
      c->setPosition({0,0});
      c->setAnchorPoint({0,1});
      auto cs = c->CCNode::getContentSize();
      auto ns = size.height+cs.height;
      c->setPositionWithUnit({0,size.height}, Unit::OpenGL, Unit::OpenGL);
      if (isVertical) {
        size.height = constraint.height!=0?std::min(cheightOpengl, ns):ns;
        size.width = std::max(size.width, cs.width);
      }
      if (isHorizontal) {
        size.width = constraint.width!=0?std::min(cwidthOpengl, ns):ns;
        size.height = std::max(size.height, cs.height);
      }
    }
    node->CCNode::setContentSize(size);
  }
};

bool FillFlowContainer::init(FillDirection dir) {
  Container::init();
  //setContentSizeWithUnit(CCSize)
  setFillDirection(dir);
  return true;
}
void FillFlowContainer::setFillDirection(FillDirection dir) {
  m_direction = dir;
  #if 0
  AxisLayout* layout = AxisLayout::create(direction == FillDirection::Horizontal || direction == FillDirection::Full ? Axis::Row : Axis::Column);
  if (direction == FillDirection::Vertical) layout->setAxisReverse(true);
  if (direction == FillDirection::Full) layout->setGrowCrossAxis(true);
  #else
  auto layout = FillFlowLayout::create();
  #endif
  setLayout(layout);
  //CCLayer::updateLayout();
};

void FillFlowContainer::addChild(CCNode* node) {
  auto con = dynamic_cast<Container*>(node);
  assert((
    "we at osu!framework requires their child to be a Container in some occasion thanks",
    con==nullptr
  ));
  CCNode::addChild(node);
  if (!isRunning()) return;
  auto cs = con->getSizeConstraints();
  // change the constraints
  // we will need a constraint with:
  // - the minimumSize is the largest minimumSize in the children
  // - the maximumSize is the smallest maximumSize in the children
  minimumSize = cs.first + minimumSize;
  maximumSize = cs.second + maximumSize;
  checkConstraints();
  CCLayer::updateLayout();
};
void FillFlowContainer::updateChildPosition() {
  auto childList = getChildren();
  if (childList) {
    CCLayer::updateLayout();
  }
};


