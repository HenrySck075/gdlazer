#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cocos2d.h>
#include <memory>

#include "gdlazer/caffeine/foundation/RenderObjectElements.hpp"
#include "gdlazer/caffeine/foundation/Widget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"

using namespace cocos2d;
using namespace geode::prelude;


// ============================================================================
// Example 2: A RenderObjectWidget that creates a CCLayerRGBA for a colored box
// ============================================================================
/// not the actual colored box so no child needed
class ColoredBoxWidget : public RenderObjectWidget {
private:
    ccColor3B m_color;
    CCSize m_size;

public:
    ColoredBoxWidget(ccColor3B color, CCSize size)
        : RenderObjectWidget(), m_color(color), m_size(size) {}

    ccColor3B getColor() const { return m_color; }
    CCSize getSize() const { return m_size; }

    std::shared_ptr<Element> createElement() override;
};

/// Custom element that creates the actual CCLayerRGBA render object
class ColoredBoxElement : public RenderObjectElement {
protected:
    cocos2d::CCNode* createRenderObject() override {
        auto widget = std::static_pointer_cast<ColoredBoxWidget>(m_widget);
        auto layer = CCLayerColor::create();
        layer->setContentSize(widget->getSize());
        layer->setColor(widget->getColor());
        layer->setOpacity(255);
        return layer;
    }

    // what was these supposed to do again?
    void insertRenderObjectChild(cocos2d::CCNode* child) override {}
    void removeRenderObjectChild(cocos2d::CCNode* child) override {}

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

        auto app = runApp(new ColoredBoxWidget({0,0,0},{50,50}));
        
        // Add it to the menu layer
        this->addChild(app, 10);
        
        // Add a label to indicate the widget is rendering
        auto label = CCLabelBMFont::create(
            "Caffeine Widget Rendering", 
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
