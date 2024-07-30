#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "MainMenuButton.hpp"

using namespace cocos2d;

class ButtonArea : public CCLayer {
private:
    std::map<std::string, std::tuple<CCLayer*,CCLayer*>> buttonsMenus;
    std::map<std::string, std::vector<MainMenuButton*>> _buttons;
    std::vector<std::string> tagsStack;
    CCPoint anchorPosition = ccp(0,0);
public:
    static ButtonArea* create(const CCPoint& anchorPos) {
        create_class(ButtonArea, init, anchorPos);
    }
    bool init(const CCPoint& anchorPos);
    void setParent(CCNode* parent) override;

    /// @brief Creates the buttons layers and optionally place them at a specific index.
    /// The first button will be placed at the left, the rest goes to the right.
    /// @param buttons 
    /// @param index the index to insert generated layers in. set to -1 to append
    void constructButtons(std::vector<MainMenuButton*>& buttons, std::string tag = "");

    void updateLayout(bool jweuioghvfweuigiweuhgiloweughiwe = true) {}
    void show(std::string tag);
    void hide(std::string tag);
    // convenient function to show the previous menu
    void pop();
    std::string getCurrent() {return tagsStack.back();}
    // just the one before getCurrent
    std::string getBefore() {return tagsStack.at(tagsStack.size()-2);}
};
