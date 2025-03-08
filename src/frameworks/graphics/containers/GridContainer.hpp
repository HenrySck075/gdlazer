#pragma once
#include "Container.hpp"
#include "GridLayout.hpp"

class GridContainer : public Container {
public:
    static GridContainer* create(const DimensionVector& columns = {Dimension::Auto()},
                               const DimensionVector& rows = {},
                               float spacing = 5.0f);
    bool init(const DimensionVector& columns,
             const DimensionVector& rows,
             float spacing);
};
