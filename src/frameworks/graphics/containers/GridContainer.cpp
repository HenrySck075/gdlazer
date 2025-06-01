#include "GridContainer.hpp"

GDF_NS_START
GridContainer* GridContainer::create(const DimensionVector& columns,
                  const DimensionVector& rows,
                  float spacing) {
  $createClass(GridContainer, init, columns, rows, spacing);
}

bool GridContainer::init(const DimensionVector& columns,
            const DimensionVector& rows,
            float spacing) {
  if (!Container::init()) return false;
  
  
  return true;
}
void GridContainer::setGap(float spacing) {
}

void GridContainer::addChildAtCell(int row, int column, Container* child) {
  if (row >= m_gridContent.size()) m_gridContent.resize(row+1, {});
  auto& gridRow = m_gridContent[row];
  if (column > gridRow.size()) gridRow.resize(column+1);
  gridRow[column] = child;
};

std::vector<float> GridContainer::getCellSizesAlongAxis(Axes axis, float spanLength) {
  const std::vector<Dimension>& dimensions = axis == Axes::X ? m_rowDimensions : m_columnDimensions;
  int sdlen = dimensions.size();
  int cellColumns = std::accumulate(m_gridContent.begin(), m_gridContent.end(), 0, [](const auto& one, const auto& two){
    return std::max(one, (int)two.size());
  });
  int cellRows = m_gridContent.size();
  int spanCount = axis == Axes::X ? cellColumns : cellRows;

  std::vector<float> sizes(spanCount, 0);
  for (int i = 0; i < spanCount || i < sdlen; i++) {

    auto dim = dimensions[i];
#define $unity(val) processUnit(val, Unit::UIKit, axis == Axes::X)
    float dimsize = $unity(dim.size);
    switch (dim.mode) {
      case Dimension::Mode::Distributed:
        sizes[i] = spanLength / sdlen;
        break;
      case Dimension::Mode::Relative:
        sizes[i] = dim.size / 100.0f * spanLength;
        break;
      case Dimension::Mode::Absolute:
        sizes[i] = dim.size;
        break;
      case Dimension::Mode::AutoSize: {
        float size = 0;

        if (axis == Axes::X) {
          // Go through each row and get the width of the cell at the indexed column
          for (int r = 0; r < cellRows; r++) {
            auto& cell = m_gridContent[r][i];
            if (!cell) continue;

            size = std::max(size, cell->getContentWidth());
          }
        } else {
          // Go through each column and get the height of the cell at the indexed row
          for (int c = 0; c < cellColumns; c++) {
            auto& cell = m_gridContent[i][c];
            if (!cell) continue;

            size = std::max(size, cell->getContentHeight());
          }
        }

        sizes[i] = size;
        break;
      }
    }
    sizes[i] = clamp(sizes[i], dim.minSize, dim.maxSize);
  }
  return sizes;
}


std::vector<float> GridContainer::distributeAlongAxis(Axes axis, float spanLength, std::vector<float> cellSizes) {
  const DimensionVector& dimensions = axis == Axes::X ? m_columnDimensions : m_rowDimensions;

  // Indices of all distributed cells
  std::vector<int> distributedIndices;
  for (int i = 0; i < cellSizes.size() || i < dimensions.size(); i++) {
    if (dimensions[i].mode == Dimension::Mode::Distributed) distributedIndices.push_back(i);
  }

  // Total number of distributed cells
  int distributionCount = distributedIndices.size();

  // The dimensions corresponding to all distributed cells
  using DimensionEntry = std::tuple<int, Dimension>;
  std::vector<DimensionEntry> distributedDimensions;
  distributedDimensions.reserve(distributionCount);
  std::transform(distributedIndices.begin(), distributedIndices.end(), std::back_inserter(distributedDimensions), [&](int i) {
    return std::make_tuple(i, i >= dimensions.size() ? Dimension() : dimensions[i]);
  });
  

  // Non-distributed size (sum of cellSizes basically)
  float requiredSize = std::accumulate(cellSizes.begin(), cellSizes.end(), 0.0f, [](float acc, float size) {
    return acc + (size > 0 ? size : 0);
  }); 

  // Distribution size for _each_ distributed cell
  float distributionSize = fmax(0, spanLength - requiredSize) / distributionCount;

  // Ordering is important to maximize excess at every step
  std::sort(distributedDimensions.begin(), distributedDimensions.end(), [](DimensionEntry a, DimensionEntry b) {
    auto& dima = std::get<1>(a);
    auto& dimb = std::get<1>(b);

    return (dima.maxSize - dima.minSize) < (dimb.maxSize - dimb.minSize);
  });
  // Write the sizes of distributed cells. 
  for (DimensionEntry& entry : distributedDimensions) {
    Dimension& dim = std::get<1>(entry);

    // Cells start off at their minimum size, and the total size should not exceed their maximum size
    cellSizes[std::get<0>(entry)] = $unity(fmin(dim.maxSize, dim.minSize + distributionSize));

    // If there's no excess, any further distributions are guaranteed to also have no excess, so this becomes a null-op
    // If there is an excess, the excess should be re-distributed among all other n-1 distributed cells
    if (--distributionCount > 0)
        distributionSize += $unity(fmax(0, distributionSize - dim.maxSize - dim.minSize)) / distributionCount;
  }
#undef $unity
  return cellSizes;
}

void GridContainer::updateLayout() {
  /// layoutContent()

  /// actually just push all cells to the container
  removeAllChildren();
  for (auto& gridRow : m_gridContent) {
    for (auto& cell : gridRow) {
      if (cell) addChild(cell);
    }
  }

  /// layoutCell()
  // completely different dimensions from getCellSizesAlongAxis??????
  const std::vector<float>& widths = distributeAlongAxis(Axes::X, getContentWidth()/* - Padding.TotalHorizontal*/, getCellSizesAlongAxis(Axes::X, getContentWidth()/* - Padding.TotalHorizontal*/));
  const std::vector<float>& heights = distributeAlongAxis(Axes::Y, getContentHeight()/* - Padding.TotalVertical*/, getCellSizesAlongAxis(Axes::Y, getContentHeight()/* - Padding.TotalVertical*/));

  for (int col = 0; col < m_gridContent.size(); col++) {
    auto& gridRow = m_gridContent[col];
    if (gridRow.empty()) continue;

    for (int row = 0; row < gridRow.size(); row++) {
      auto& cell = gridRow[row];
      if (!cell) continue;

      cell->setContentSize({widths[col], heights[row]});

      float x = getPositionX(), y = getPositionY();
      if (col > 0)
        x = tryGetCell(row, col - 1)->getPositionX() + tryGetCell(row, col - 1)->getContentWidth();

      if (row > 0)
        y = tryGetCell(row - 1, col)->getPositionY() + tryGetCell(row - 1, col)->getContentHeight();
      
      cell->setPosition({x,y});
      cell->setAnchor(Anchor::BottomLeft);
    }
  }
}

GDF_NS_END
