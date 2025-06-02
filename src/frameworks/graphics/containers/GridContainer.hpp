#pragma once
#include "Container.hpp"

GDF_NS_START
struct Dimension {
  enum class Mode {
    /// Any remaining area of the <see cref="GridContainer"/> will be divided amongst this and all
    /// other elements which use <see cref="Distributed"/>.
    Distributed,

    /// This element should be sized relative to the dimensions of the <see cref="GridContainer"/>.
    Relative,

    /// This element has a size independent of the <see cref="GridContainer"/>.
    Absolute,

    /// This element will be sized to the maximum size along its span.
    AutoSize
  };

  Mode mode = Mode::AutoSize;
  float size = 0;
  /// TODO: std::numeric_limits<T>::infinity ??????
  float minSize = 0;
  float maxSize = 0;
};

/// for no reason
enum class Axes {
  X, Y
};

using DimensionVector = std::vector<Dimension>;

class GridContainer : public Container {
public:
  static GridContainer* create(const DimensionVector& columns = {Dimension{}},
                 const DimensionVector& rows = {},
                 float spacing = 5.0f);
  bool init(const DimensionVector& columns,
      const DimensionVector& rows,
      float spacing);
  void setGap(float spacing);

  void addChildAtCell(int row, int column, Container* child);
  void reserve(int rows, int columns);

  void updateLayout();

private:
  std::vector<float> getCellSizesAlongAxis(Axes axis, float spanLength);
  std::vector<float> distributeAlongAxis(Axes axis, float spanLength, std::vector<float> cellSizes);
  Container* tryGetCell(int row, int col);
  std::vector<std::vector<geode::Ref<Container>>> m_gridContent;
  DimensionVector m_columnDimensions;
  DimensionVector m_rowDimensions;
};
GDF_NS_END
