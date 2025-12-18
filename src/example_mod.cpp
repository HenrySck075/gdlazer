#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cocos2d.h>
#include <memory>

#include "gdlazer/caffeine/widgets/framework/ComponentElements.hpp"
#include "gdlazer/caffeine/foundation/utils/Ref.hpp"
#include "gdlazer/caffeine/widgets/Center.hpp"
#include "gdlazer/caffeine/widgets/SizedBox.hpp"
#include "gdlazer/caffeine/widgets/framework/State.hpp"
#include "gdlazer/caffeine/widgets/framework/Widget.hpp"
#include "gdlazer/caffeine/widgets/ColoredBox.hpp"
#include "gdlazer/caffeine/widgets/WidgetsContainer.hpp"

using namespace cocos2d;
using namespace geode::prelude;

// ============================================================================
// Hook into MenuLayer to add our Caffeine widget example
// ============================================================================

class gamerState : public caffeine::State {
  caffeine::Color m_color;
  std::thread m_thread;
public:
  void initState() override {
    // A thread that sleeps for 1 second and roll a random color in a setState callback
    m_thread = std::thread([this]() {
      while (true) {
        /// geode's queue is called before our framework's pipeline cycle so its technically still on the previous frame
        geode::queueInMainThread([this]{
          this->setState([this]() {
            // Random color
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;
            m_color = caffeine::Color::fromRGB(r, g, b);
          });
        });
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });
  }
  caffeine::Widget* build(caffeine::RefNauseam<caffeine::BuildContext> context) override {
    // Build a simple widget tree: Padding(16px) -> ColoredBox(blue)
    return new caffeine::SizedBox(
      {200,200}, 
      new caffeine::ColoredBox(m_color)
    );
  }
};

class gamer : public caffeine::StatefulWidget {
public:
  caffeine::RefNauseam<caffeine::State> createState() override {
    return new gamerState();
  }
};

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        // Build the widget tree following the catgirl's guide:
        // Padding(16px) -> ColoredBox(blue)
        auto paddedBox = new caffeine::Center(new gamer());

        auto app = runApp(paddedBox);

        // Add the widget system to the menu layer
        this->addChild(app, 10);
/*
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
*/
        return true;
    }
};
