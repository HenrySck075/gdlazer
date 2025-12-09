#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cocos2d.h>
#include <memory>

#include "gdlazer/caffeine/foundation/RenderObjectElements.hpp"
#include "gdlazer/caffeine/foundation/Widget.hpp"
#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/BuildOwner.hpp"
#include "gdlazer/caffeine/widgets/RootWidget.hpp"
#include "gdlazer/caffeine/widgets/WidgetsBinding.hpp"

using namespace cocos2d;
using namespace geode::prelude;

// ============================================================================
// Example 1: A simple StatelessWidget that creates a colored CCNode
// ============================================================================

class ColoredBoxWidget : public StatelessWidget {
private:
    ccColor3B m_color;
    CCSize m_size;

public:
    ColoredBoxWidget(ccColor3B color, CCSize size) 
        : m_color(color), m_size(size) {}

    Widget* build(std::shared_ptr<BuildContext> context) override {
        // For now, return nullptr - we'll render directly in the RenderObjectWidget
        return nullptr;
    }
};

// ============================================================================
// Example 2: A RenderObjectWidget that creates a CCDrawNode for rendering
// ============================================================================

class BoxRenderWidget : public SingleChildRenderObjectWidget {
private:
    ccColor3B m_color;
    CCSize m_size;

public:
    BoxRenderWidget(ccColor3B color, CCSize size, shared_ptr_ctor<Widget> child = nullptr)
        : SingleChildRenderObjectWidget(child), m_color(color), m_size(size) {}

    std::shared_ptr<Element> createElement() override {
        return std::make_shared<SingleChildRenderObjectElement>(
            this
        );
    }
};

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
