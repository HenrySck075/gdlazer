#pragma once

#include "Container.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>
using namespace cocos2d;
#include "../../utils/readonly.hpp"

enum class GridSizeMode {
    /// Any remaining area of the GridContainer will be divided amongst this and all
    /// other elements which use Distributed.
    Distributed,

    /// This element should be sized relative to the dimensions of the GridContainer.
    Relative,

    /// This element has a size independent of the GridContainer.
    Absolute,

    /// This element will be sized to the maximum size along its span.
    AutoSize
};

/// @note Size values are parsed in OpenGL size
struct Dimension {
    /// <summary>
    /// The mode in which this row or column <see cref="GridContainer"/> is sized.
    /// </summary>
    readonly<GridSizeMode, Dimension> mode;

    /// <summary>
    /// The size of the row or column which this <see cref="Dimension"/> applies to.
    /// Only has an effect if <see cref="Mode"/> is not <see cref="GridSizeMode.Distributed"/>.
    /// </summary>
    readonly<float, Dimension> size;

    /// <summary>
    /// The minimum size of the row or column which this <see cref="Dimension"/> applies to.
    /// </summary>
    readonly<float, Dimension> minSize;

    /// <summary>
    /// The maximum size of the row or column which this <see cref="Dimension"/> applies to.
    /// </summary>
    readonly<float, Dimension> maxSize;

    Dimension(
        GridSizeMode mode_ = GridSizeMode::Distributed, 
        float size_ = 0, 
        float minSize_ = 0, 
        float maxSize_ = -1
    ) : mode(decltype(mode)(mode_)), size(decltype(size)(size_)), minSize(decltype(minSize)(minSize_)), maxSize(decltype(maxSize)(maxSize_)) {};

    bool operator==(const Dimension& c) const {
        return (
            c.mode == mode &&
            c.size == size &&
            c.minSize == minSize &&
            c.maxSize == maxSize
        );
    }
};

// geode team baited everyone in the docs
class GridLayout : public ContainerLayout {
    std::vector<Dimension> colDim;
    std::vector<Dimension> rowDim;
public:
    bool init(decltype(colDim) col, decltype(rowDim) row) {
        colDim = col;
        rowDim = row;
        return true;
    };
    static GridLayout* create(std::vector<Dimension> columnDimensions = {Dimension()}, std::vector<Dimension> rowDimensions = {Dimension()}) {
        // same assert but on a different class to distinguish this with GridContainer
        // (incase someone uses gridlayout for non-containers)
        assert(("[GridLayout]: Dimensions list must have at least 1 dimension. (which i did but you just passed in an empty vector)", columnDimensions.size() == 0 || rowDimensions.size() == 0));
        create_class(GridLayout, init, columnDimensions, rowDimensions);
    }
    void apply(CCNode* on) override;
    CCSize getSizeHint(CCNode* on) const override {return on->CCNode::getContentSize();};
};

// wrapper container around geode's layout system
class GridContainer : public Container {
public:
    bool init(std::vector<Dimension> columnDimensions, std::vector<Dimension> rowDimensions);
    static GridContainer* create(std::vector<Dimension> columnDimensions = {Dimension()}, std::vector<Dimension> rowDimensions = {Dimension()}) {
        assert(("[GridContainer]: Dimensions list must have at least 1 dimension. (which i did but you just passed in an empty vector)", columnDimensions.size() == 0 || rowDimensions.size() == 0));
        create_class(GridContainer,init,columnDimensions,rowDimensions);
    }
    void updateLayout() {
        CCLayer::updateLayout();
    }
    void addChild(CCNode* child) {
        CCLayer::addChild(child);
        updateLayout();
    }
};