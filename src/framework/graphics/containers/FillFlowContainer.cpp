#include "FillFlowContainer.hpp"


void FillFlowLayout::apply(CCNode* on) {
    CCArrayExt<Container*> nodes = getNodesToPosition(on);

    auto node = dynamic_cast<Container*>(on);
    assert(("l bozo", node==nullptr));

    auto constraint = node->getSizeConstraints().second;
    CCSize size = {0,0};
    for (auto c : nodes) {
        //log::debug("[FillFlowLayout]: {}",size.height);
        if (c->getSizeUnit().second == Unit::Percent && constraint.height == 0) {
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
        c->setPositionWithUnit({0,size.height}, Unit::OpenGL, Unit::OpenGL);
        auto ns = size.height+cs.height;
        size.height = constraint.height!=0?std::min(constraint.height, ns):ns;
    }
    size.width = node->CCNode::getContentSize().width;
    node->CCNode::setContentSize(size);
}

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
    assert((
        "we at osu!framework requires their child to be a Container in some occasion thanks",
        con==nullptr
    ));
    CCNode::addChild(node);
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


