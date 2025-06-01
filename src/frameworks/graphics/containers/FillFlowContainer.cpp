#include "FillFlowContainer.hpp"

GDF_NS_START

FillFlowContainer* FillFlowContainer::create(FillDirection direction) {
    $createClass(FillFlowContainer, init, direction);
}
bool FillFlowContainer::init(FillDirection direction) {
    if (!Container::init()) return false;
    
    m_layout = FillFlowLayout::create(direction);
    m_layout->setGap(5.0f); // default gap
    this->setLayout(m_layout);
    
    return true;
}
void FillFlowContainer::setGap(float gap) {
    m_layout->setGap(gap);
}
float FillFlowContainer::getGap() {
    return m_layout->getGap();
}
GDF_NS_END
