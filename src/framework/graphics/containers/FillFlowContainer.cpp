#include "FillFlowContainer.hpp"

bool FillFlowContainer::init(FillDirection dir) {
    Container::init();
    //setContentSizeWithUnit(CCSize)
    setFillDirection(dir);
    return true;
}
void FillFlowContainer::setFillDirection(FillDirection dir) {
    direction = dir;
    /*
    AxisLayout* layout = AxisLayout::create(direction == FillDirection::Horizontal || direction == FillDirection::Full ? Axis::Row : Axis::Column);
    if (direction == FillDirection::Vertical) layout->setAxisReverse(true);
    if (direction == FillDirection::Full) layout->setGrowCrossAxis(true);
    */
    auto layout = FillFlowLayout::create();
    setLayout(layout);
    //CCLayer::updateLayout();
};

void FillFlowContainer::addChild(CCNode* node) {
    auto con = dynamic_cast<Container*>(node);
    assert(("we at osu!framework requires their child to be a Container in some occasion thanks",con==nullptr));
    CCNode::addChild(node);
    auto cs = con->getSizeConstraints();
    // change the constaints
    // we will need a constraint with:
    // - the minimumSize is the largest minimumSize in the children
    // - the maximumSize is the smallest maximumSize in the children
    minimumSize = std::max(cs.first, minimumSize);
    maximumSize = std::min(cs.second, maximumSize);
    checkConstraints();
    CCLayer::updateLayout();
};
void FillFlowContainer::updateChildPosition() {
    auto childList = getChildren();
    if (childList) {
        for (auto* c : CCArrayExt<CCNode*>(childList)) {
            if (auto cc = typeinfo_cast<Container*>(c)) {
                cc->dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
                cc->setPositionUnit(Unit::OpenGL, Unit::OpenGL);
            }
        }
        CCLayer::updateLayout();
    }
};


