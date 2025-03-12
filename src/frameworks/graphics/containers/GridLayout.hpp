#pragma once
#include <Geode/ui/Layout.hpp>

#include <vector>

struct Dimension {
    enum class Mode {
        Fixed,      // Absolute size in pixels
        Relative,   // Size relative to container (0.0-1.0)
        Auto       // Size based on content
    };

    Mode mode;
    float size;

    float resolve(float containerSize, float contentSize) const {
        switch (mode) {
            case Mode::Fixed: return size;
            case Mode::Relative: return containerSize * size;
            case Mode::Auto: return contentSize;
            default: return 0.0f;
        }
    }
};

using DimensionVector = std::vector<Dimension>;

class GridLayout : public geode::Layout {
protected:
    DimensionVector m_columnDimensions;
    DimensionVector m_rowDimensions;
    float m_spacing;

public:
    static GridLayout* create(const DimensionVector& columns, 
                            const DimensionVector& rows = {});
    GridLayout(const DimensionVector& columns, 
             const DimensionVector& rows);
    GridLayout *setGap(float gap);
    float getGap() const;
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override {
        // didn't see this function used anywhere in geode so
        return on->getContentSize();
    };
    void apply(cocos2d::CCNode* on) override;

private:
    std::vector<float> calculateSizes(const DimensionVector& dimensions, 
                                    float containerSize,
                                    const std::vector<float>& contentSizes) const;
};
