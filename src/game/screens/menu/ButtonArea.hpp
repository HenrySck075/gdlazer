#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../../framework/graphics/containers/Container.hpp"
#include "MainMenuButton.hpp"
#include <henrysck075.easings/include/easings.hpp>

using namespace cocos2d;

class ButtonArea final : public Container {
private:
    CCDictionaryExt<std::string, Container*> buttonsMenus;
    CCDictionaryExt<std::string, CCArray*> _buttons;
    std::vector<std::string> tagsStack;
    CCPoint anchorPosition = {0,0};
    float animationSpeed = 0.5;
    using EasingEffect = easingsActions::CCEaseExponentialOut;
    CCLayerColor* colorBg;

    bool hidden = true;

    int curZOrder = 0;
public:
    static ButtonArea* create(const CCPoint& anchorPos) {
        create_class(ButtonArea, init, anchorPos);
    }
    bool init(const CCPoint& anchorPos);

    bool dispatchEvent(NodeEvent* e) override {
        if (e->eventName()!="keyboardEvent") return Container::dispatchEvent(e);
        else {
            auto cur = getCurrent();
            if (cur.has_value()) {
                dispatchToChildInList(e, getChildByIDRecursive("buttonarea_"+cur.value())->getChildren());
            }
            return true;
        }
    };

    /// @brief Creates the buttons layers and optionally place them at a specific index.
    /// The first button will be placed at the left, the rest goes to the right.
    /// @param buttons The buttons.
    /// @param index the index to insert generated layers in. set to -1 to append
    void constructButtons(CCArrayExt<MainMenuButton*> buttons, std::string tag = "");

    void updateLayout(bool jweuioghvfweuigiweuhgiloweughiwe = true) {}
    void show(std::string tag);
    void hide(std::string tag, bool collapse = true, bool close = false);
    // convenient function to show the previous menu
    void pop() {
        auto d = getBefore();
        if (d.has_value()) show(d.value());
    };
    std::optional<std::string> getCurrent() {
        if (tagsStack.size() == 0) return std::nullopt;
        return std::optional(tagsStack.at(tagsStack.size()-1));
    }
    // just the one before getCurrent
    std::optional<std::string> getBefore() {
        if (tagsStack.size() <= 1) return std::nullopt;

        return std::optional(tagsStack.at(tagsStack.size()-2));
    }
};
