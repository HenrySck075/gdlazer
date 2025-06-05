#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "MainMenuButton.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

using namespace cocos2d;

GDL_NS_START
class ButtonArea final : public frameworks::Container {
private:
  CCDictionaryExt<std::string, Container*> buttonsMenus;
  CCDictionaryExt<std::string, CCArray*> _buttons;
  std::vector<std::string> tagsStack;
  CCPoint anchorPosition = {0,0};
  float animationSpeed = 0.5;
  geode::Ref<CCLayerColor> colorBg;

  bool hidden = true;

  int curZOrder = 0;
public:
  static ButtonArea* create(const CCPoint& anchorPos) {
    $createClass(ButtonArea, init, anchorPos);
  }
  bool init(const CCPoint& anchorPos);

  //bool doDispatchEvent(frameworks::Event *e, std::type_index) override;

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
GDL_NS_END
