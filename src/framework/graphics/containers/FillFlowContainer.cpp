#include "FillFlowContainer.hpp"

bool FillFlowContainer::init(FillDirection dir) {
    Container::init();
    setFillDirection(dir);
    return true;
}
void FillFlowContainer::setFillDirection(FillDirection dir) {
    direction = dir;
    AxisLayout* layout = AxisLayout::create(direction == FillDirection::Horizontal || direction == FillDirection::Full ? Axis::Row : Axis::Column);
    if (direction == FillDirection::Full) layout = layout->setGrowCrossAxis(true);
    setLayout(layout);
    updateChildPosition();
};

void FillFlowContainer::updateChildPosition() {

    auto childList = getChildren();
    if (childList) {
        for (auto* c : CCArrayExt<CCNode*>(childList)) {
            if (auto cc = typeinfo_cast<Container*>(c)) cc->setPositionUnit(Unit::OpenGL, Unit::OpenGL);
        }
        CCLayer::updateLayout();
    }
};


