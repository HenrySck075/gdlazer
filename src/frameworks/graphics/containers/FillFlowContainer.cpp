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
[[clang::optnone]]
bool FillFlowContainer::init(FillDirection direction) {
    if (!Container::init()) return false;
    
    auto layout = FillFlowLayout::create(direction);
    layout->setGap(5.0f); // default gap
    this->setLayout(layout);
    
    return true;
}
GDF_NS_END
