#include "GridContainer.hpp"

GDL_NS_START
GridContainer* GridContainer::create(const DimensionVector& columns,
                                  const DimensionVector& rows,
                                  float spacing) {
    auto ret = new GridContainer();
    if (ret && ret->init(columns, rows, spacing)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GridContainer::init(const DimensionVector& columns,
                      const DimensionVector& rows,
                      float spacing) {
    if (!Container::init()) return false;
    
    auto layout = GridLayout::create(columns, rows)->setGap(spacing);
    this->setLayout(layout);
    
    return true;
}
GDL_NS_END