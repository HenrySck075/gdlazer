#pragma once
#include "Container.hpp"
#include "GridLayout.hpp"

GDF_NS_START
class GridContainer : public Container {
public:
    static GridContainer* create(const DimensionVector& columns = {Dimension{.mode = Dimension::Mode::Auto}},
                               const DimensionVector& rows = {},
                               float spacing = 5.0f);
    bool init(const DimensionVector& columns,
             const DimensionVector& rows,
             float spacing);
};
GDF_NS_END
