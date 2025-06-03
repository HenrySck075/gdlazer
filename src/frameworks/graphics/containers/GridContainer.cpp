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
  
  m_columnDimensions = columns;
  m_rowDimensions = rows;
  
  return true;
}
void GridContainer::setGap(float spacing) {
  m_gap = spacing;
}

void GridContainer::addChildAtCell(int row, int column, Container* child) {
  if (row >= m_gridContent.size()) m_gridContent.resize(row+1, {});
  auto& gridRow = m_gridContent[row];
  if (column >= gridRow.size()) gridRow.resize(column+1, nullptr);
  gridRow[column] = child;
};

void GridContainer::reserve(int rows, int columns) {
  std::vector<geode::Ref<Container>> c(columns, nullptr);
  m_gridContent.resize(rows, c);
}

std::vector<float> GridContainer::getCellSizesAlongAxis(Axes axis, float spanLength) {
  log::debug("[GridContainer]: spanlen: {}", spanLength);
  bool xAxis = axis == Axes::X;
  const std::vector<Dimension>& dimensions = xAxis ? m_columnDimensions : m_rowDimensions;
  int sdlen = dimensions.size();
  int cellColumns = std::accumulate(m_gridContent.begin(), m_gridContent.end(), 0, [](const auto& one, const auto& two){
    return std::max(one, (int)two.size());
  });
  int cellRows = m_gridContent.size();
  log::debug("[GridContainer]: querying cell sizes along the {} axis ({} {} {})", xAxis ? 'x' : 'y', cellColumns, cellRows, sdlen);
  int spanCount = xAxis ? cellColumns : cellRows;

  std::vector<float> sizes(spanCount, 0);
  for (int i = 0; i < sdlen && i < spanCount; i++) {
    auto dim = dimensions[i];
#define $unity(val) processUnit(val, Unit::UIKit, xAxis)
    float dimsize = $unity(dim.size);
    float gapAlloc =  m_gap /* * ((i == 0 || i == sdlen || i == spanCount) ? 1 : 2)*/;
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

        if (xAxis) {
          // Go through each row and get the width of the cell at the indexed column
          for (int r = 0; r < cellRows; r++) {
            auto cell = tryGetCell(r,i);
            if (cell)
              size = std::max(size, cell->getContentSize().width);
          }
        } else {
          // Go through each column and get the height of the cell at the indexed row
          for (int c = 0; c < cellColumns; c++) {
            auto cell = tryGetCell(i,c);
            if (cell)
              size = std::max(size, cell->getContentSize().height);
          }
        }

        sizes[i] = size;
        break;
      }
    }
    log::debug("index {}: {}",
      i,
      sizes[i] = clamp(sizes[i], fmax(dim.minSize,0), dim.maxSize == 0 ? sizes[i] : dim.maxSize)
    );
  }
  return sizes;
}


/// I'm not sure how does this even work
std::vector<float> GridContainer::distributeAlongAxis(Axes axis, float spanLength, std::vector<float> cellSizes) {
  return cellSizes;

  /// Everything after this is unused

  const DimensionVector& dimensions = axis == Axes::X ? m_columnDimensions : m_rowDimensions;

  // Indices of all distributed cells
  std::vector<int> distributedIndices;
  int ds = dimensions.size();
  for (int i = 0; i < ds && i < cellSizes.size(); i++) {
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
    return acc + fmax(0, size);
  }); 

  if (distributionCount == 0) return cellSizes;
  // Distribution size for _each_ distributed cell
  float distributionSize = fmax(0, requiredSize) / distributionCount;

  log::debug("[GridContainer]: rs|ds-dc: {}|{}-{}", requiredSize, distributionSize, distributionCount);

  // Ordering is important to maximize excess at every step
  std::sort(distributedDimensions.begin(), distributedDimensions.end(), [](DimensionEntry a, DimensionEntry b) {
    auto& dima = std::get<1>(a);
    auto& dimb = std::get<1>(b);

    return (dima.maxSize - dima.minSize) < (dimb.maxSize - dimb.minSize);
  });
  bool xAxis = axis == Axes::X;
  // Write the sizes of distributed cells. 
  for (DimensionEntry& entry : distributedDimensions) {
    Dimension& dim = std::get<1>(entry);

    // Cells start off at their minimum size, and the total size should not exceed their maximum size
    float size = dim.minSize + distributionSize;
    cellSizes[std::get<0>(entry)] = $unity(dim.maxSize == 0 ? size : fmin(dim.maxSize, size));

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

  CCSize contentSize {0,0};
  for (int row = 0; row < m_gridContent.size(); row++) {
    auto& gridCol = m_gridContent[row];
    if (gridCol.empty()) continue;

    for (int col = 0; col < gridCol.size(); col++) {
      auto& cell = gridCol[col];
      if (!cell) continue;
      
      CCSize s{widths[col], heights[row]};
      log::debug("[GridContainer]: {},{}: {}", col, row, s);
      cell->setContentSize(s);

      float x = 0, y = 0;
      if (row > 0)
        y = tryGetCell(row - 1, col)->getPositionY() + tryGetCell(row - 1, col)->getContentHeight() + m_gap;

      if (col > 0)
        x = tryGetCell(row, col - 1)->getPositionX() + tryGetCell(row, col - 1)->getContentWidth() + m_gap;
      
      cell->setPosition({x,y});
      //cell->setAnchor(Anchor::TopLeft);
      auto newContentSize = cell->getContentSize() + cell->getPosition();

      contentSize.setSize(
        fmax(contentSize.width, newContentSize.width), 
        fmax(contentSize.height, newContentSize.height)
      );
    }
  }
  setContentSize(contentSize);
  
  /// Loop through them one more time to reanchor the cells
  for (auto& gridRow : m_gridContent) {
    for (auto& cell : gridRow) {
      if (!cell) continue;
      cell->setAnchor(Anchor::TopLeft);
      cell->setAnchorPoint({0,1});
    }
  }

}

geode::Ref<Container> g_dummyContainer;

Container* GridContainer::tryGetCell(int row, int col) {
  if (!g_dummyContainer) g_dummyContainer = Container::create();
  if (row >= m_gridContent.size()) {
    log::warn("[GridContainer::tryGetCell]: Row index out of bounds: {} >= {}", row, m_gridContent.size());
    return g_dummyContainer;
  }
  auto& cols = m_gridContent[row];
  if (col >= cols.size()) {
    log::warn("[GridContainer::tryGetCell]: Column index out of bounds: {} >= {}", col, cols.size());
    return g_dummyContainer;
  }
  return cols[col];
}

GDF_NS_END
