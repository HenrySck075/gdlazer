#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "MainMenuButton.hpp"

using namespace cocos2d;

class ButtonArea : public CCLayer {
private:
    std::vector<std::tuple<CCLayer*,CCLayer*>> buttonsMenus;
    std::vector<std::vector<MainMenuButton*>> _buttons;
    CCPoint anchorPosition = ccp(0,0);
    int shownIndex = -1;
public:
    static ButtonArea* create(const CCPoint& anchorPos) {
        create_class(ButtonArea, init, anchorPos);
    }
    bool init(const CCPoint& anchorPos);
    void setParent(CCNode* parent) override;

    /// @brief g
    /// @param buttons 
    /// @param index the index to insert generated layers in. set to -1 to append
    void constructButtons(std::vector<MainMenuButton*>& buttons, int index = -1);

    void updateLayout(bool jweuioghvfweuigiweuhgiloweughiwe = true) {}
    void show(int index);
    void hide(int index);
};