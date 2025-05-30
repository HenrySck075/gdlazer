#include "FillFlowContainer.hpp"
#include "FillFlowLayout.hpp"

GDF_NS_START

FillFlowContainer* FillFlowContainer::create(FillDirection direction) {
    auto ret = new FillFlowContainer();
    if (ret && ret->init(direction)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
bool FillFlowContainer::init(FillDirection direction) {
    if (!Container::init()) return false;
    
    auto layout = FillFlowLayout::create(direction);
    layout->setGap(5.0f); // default gap
    this->setLayout(layout);
    
    return true;
}
void FillFlowContainer::setGap(float gap) {
    static_cast<FillFlowLayout*>(this->getLayout())->setGap(gap);
}
float FillFlowContainer::getGap() {
    return static_cast<FillFlowLayout*>(getLayout())->getGap();
}
GDF_NS_END
