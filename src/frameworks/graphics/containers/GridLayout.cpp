#include "GridLayout.hpp"
#include "../containers/Container.hpp"

GridLayout* GridLayout::create(const DimensionVector& columns, 
               const DimensionVector& rows) {
  auto ret = new GridLayout(columns, rows);
  ret->autorelease();
  return ret;
}

GridLayout::GridLayout(const DimensionVector& columns, 
           const DimensionVector& rows) {
  m_columnDimensions = columns;
  m_rowDimensions = rows;
}

std::vector<float> GridLayout::calculateSizes(const DimensionVector& dimensions, 
                      float containerSize,
                      const std::vector<float>& contentSizes) const {
  std::vector<float> sizes(dimensions.size());
  float remainingSpace = containerSize - (m_spacing * (dimensions.size() - 1));
  float totalRelative = 0.0f;
  std::vector<size_t> relativeIndexes;
  int autoCount = 0;
  std::vector<size_t> autoIndexes;

  // fixed size calc (and also exclude relative & auto for later)
  for (size_t i = 0; i < dimensions.size(); ++i) {
    const auto& dim = dimensions[i];
    if (dim.mode == Dimension::Mode::Fixed) {
      sizes[i] = dim.size;
      remainingSpace -= dim.size;
    } else if (dim.mode == Dimension::Mode::Relative) {
      totalRelative += dim.size;
      relativeIndexes.push_back(i);
    } else {
      autoCount++;
      autoIndexes.push_back(i);
    }
  }

  // relative sizes
  if (totalRelative > 0) {
    float spaceForRelative = remainingSpace * (autoCount > 0 ? 0.8f : 1.0f);
    for (size_t i : relativeIndexes) {
      const auto& dim = dimensions[i];
      sizes[i] = spaceForRelative * (dim.size / totalRelative);
      remainingSpace -= sizes[i];
    }
  }

  // auto sizes
  if (autoCount > 0) {
    float autoSize = remainingSpace / autoCount;
    for (size_t i : autoIndexes) {
      sizes[i] = std::max(autoSize, contentSizes[i]);
    }
  }

  return sizes;
}

void GridLayout::apply(cocos2d::CCNode* on) {
  auto container = dynamic_cast<Container*>(on);
  if (!container) return;

  auto children = container->getChildren();
  if (!children) return;

  // Ensure we have at least one column and row
  if (m_columnDimensions.empty()) {
    m_columnDimensions = {Dimension{.mode = Dimension::Mode::Auto}};
  }
  if (m_rowDimensions.empty() && children) {
    int rowCount = (children->count() + m_columnDimensions.size() - 1) / m_columnDimensions.size();
    m_rowDimensions = std::vector<Dimension>(rowCount, {.mode = Dimension::Mode::Auto});
  }

  // Calculate content sizes for auto dimensions
  std::vector<float> colContentSizes(m_columnDimensions.size(), 0.0f);
  std::vector<float> rowContentSizes(m_rowDimensions.size(), 0.0f);

  int col = 0, row = 0;
  for (auto* child : geode::cocos::CCArrayExt<cocos2d::CCNode>(children)) {
    if (!child || !child->isVisible()) continue;
    
    colContentSizes[col] = std::max(colContentSizes[col], child->getContentSize().width);
    rowContentSizes[row] = std::max(rowContentSizes[row], child->getContentSize().height);

    col++;
    if (col >= m_columnDimensions.size()) {
      col = 0;
      row++;
    }
  }

  // Calculate final sizes
  auto colSizes = calculateSizes(m_columnDimensions, container->getContentSize().width, colContentSizes);
  auto rowSizes = calculateSizes(m_rowDimensions, container->getContentSize().height, rowContentSizes);

  // Position children
  col = 0;
  row = 0;
  float x = 0;
  float y = 0;

  for (auto* child : geode::cocos::CCArrayExt<cocos2d::CCNode>(children)) {
    if (!child || !child->isVisible()) continue;

    child->setPosition(x, y);
    child->setContentSize({colSizes[col], rowSizes[row]});

    x += colSizes[col] + m_spacing;
    col++;
    
    if (col >= m_columnDimensions.size()) {
      col = 0;
      row++;
      x = 0;
      y += rowSizes[row - 1] + m_spacing;
    }
  }
}
GridLayout *GridLayout::setGap(float gap) {
  m_spacing = gap;
  return this;
}
float GridLayout::getGap() const { return m_spacing; }
