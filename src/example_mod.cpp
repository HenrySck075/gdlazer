#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cocos2d.h>
#include <memory>

#include "gdlazer/caffeine/foundation/RenderObjectElements.hpp"
#include "gdlazer/caffeine/foundation/Widget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/RenderObject.hpp"
#include "gdlazer/caffeine/widgets/Padding.hpp"
#include "gdlazer/caffeine/widgets/WidgetsContainer.hpp"

using namespace cocos2d;
using namespace geode::prelude;

// ============================================================================
// Example Layout: Inspired by the Catgirl's Guide from LAYOUT_SYSTEM.md
// A simple colored box with padding, demonstrating the layout system
// ============================================================================

namespace {

/// Concrete RenderObject for a colored box
class ColoredBoxRender : public caffeine::RenderBox {
private:
    ccColor3B m_color;

public:
    ColoredBoxRender(ccColor3B color) : m_color(color) {}

    void performLayout() override {
        m_size = m_constraints.biggest();
    }

    void paint(SkCanvas* canvas) override {
        // Paint a colored rectangle using Skia
        SkPaint paint;
        paint.setColor(SkColorSetARGB(255, m_color.r, m_color.g, m_color.b));
        canvas->drawRect(SkRect::MakeWH(m_size.width, m_size.height), paint);
    }
};


}  // namespace

// ============================================================================
// Widgets - Map to RenderObjects
// ============================================================================

/// A colored box widget that renders a solid color rectangle
class ColoredBoxWidget : public RenderObjectWidget {
private:
    ccColor3B m_color;

public:
    ColoredBoxWidget(ccColor3B color) : m_color(color) {}

    ccColor3B getColor() const { return m_color; }

    std::shared_ptr<Element> createElement() override;
};

/// Element for the colored box
class ColoredBoxElement : public RenderObjectElement {
protected:
    std::shared_ptr<caffeine::RenderObject> createRenderObject() override {
        auto widget = std::static_pointer_cast<ColoredBoxWidget>(m_widget);
        return std::make_shared<ColoredBoxRender>(widget->getColor());
    }

    void insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override {}
    void removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override {}

public:
    using RenderObjectElement::RenderObjectElement;
};

inline std::shared_ptr<Element> ColoredBoxWidget::createElement() {
    return std::make_shared<ColoredBoxElement>(this);
}


// ============================================================================
// Hook into MenuLayer to add our Caffeine widget example
// ============================================================================

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        // Build the widget tree following the catgirl's guide:
        // Padding(16px) -> ColoredBox(blue)
        auto blueBox = new ColoredBoxWidget({100, 150, 200});
        auto paddedBox = new caffeine::Padding(16.0f, blueBox);

        auto app = runApp(paddedBox);

        // Add the widget system to the menu layer
        this->addChild(app, 10);

        // Add a label to indicate the framework is active
        auto label = CCLabelBMFont::create(
            "Caffeine Skia Rendering Active",
            "bigFont.fnt"
        );
        label->setPosition(CCDirector::sharedDirector()->getWinSize().width / 2,
                          CCDirector::sharedDirector()->getWinSize().height - 100);
        label->setScale(0.5f);
        label->setColor({0, 255, 0});
        this->addChild(label, 11);

        return true;
    }
};
