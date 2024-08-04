#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "MainMenuButton.hpp"

using namespace cocos2d;

class ButtonArea : public CCLayer {
private:
    CCDictionaryExt<std::string, CCLayer*> buttonsMenus;
    CCDictionaryExt<std::string, CCArray*> _buttons;
    std::vector<std::string> tagsStack;
    CCPoint anchorPosition = ccp(0,0);
    float animationSpeed = 0.5;
    using EasingEffect = CCEaseExponentialOut;
public:
    static ButtonArea* create(const CCPoint& anchorPos) {
        create_class(ButtonArea, init, anchorPos);
    }
    bool init(const CCPoint& anchorPos);
    void setParent(CCNode* parent) override;

    /// @brief Creates the buttons layers and optionally place them at a specific index.
    /// The first button will be placed at the left, the rest goes to the right.
    /// @param buttons The buttons.
    /// @param index the index to insert generated layers in. set to -1 to append
    void constructButtons(CCArrayExt<MainMenuButton*> buttons, std::string tag = "");

    void updateLayout(bool jweuioghvfweuigiweuhgiloweughiwe = true) {}
    void show(std::string tag);
    void hide(std::string tag, bool collapse = true);
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
