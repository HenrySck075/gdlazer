#include "FillFlowContainer.hpp"

GDF_NS_START

FillFlowContainer* FillFlowContainer::create(FillDirection direction, geode::Anchor anchor) {
    $createClass(FillFlowContainer, init, direction, anchor);
}
bool FillFlowContainer::init(FillDirection direction, geode::Anchor anchor) {
    if (!Container::init()) return false;
    
    m_layout = FillFlowLayout::create(direction, anchor);
    m_layout->setGap(5.0f); // default gap
    this->setLayout(m_layout);
    setAutoGrowAxis(true);
    
    return true;
}
void FillFlowContainer::setGap(float gap) {
    m_layout->setGap(gap);
}
float FillFlowContainer::getGap() {
    return m_layout->getGap();
}
void FillFlowContainer::setAxisReverse(bool axisReverse) {
    m_layout->setAxisReverse(axisReverse);
}
void FillFlowContainer::setAutoGrowAxis(bool enable) {
    m_layout->setAutoGrowAxis(enable ? std::make_optional(7) : std::nullopt);
};
void FillFlowContainer::setChildAnchor(geode::Anchor anchor) {
    m_layout->setAnchor(anchor);
}
GDF_NS_END
