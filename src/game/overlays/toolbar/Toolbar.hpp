#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../frameworks/graphics/containers/VisibilityContainer.hpp"
#include "../../graphics/CCLayerGradient2.hpp"
#include "ToolbarConstants.hpp"
#include "../../../helpers/CustomActions.hpp"

using namespace geode::prelude;

GDL_NS_START

class Toolbar : public frameworks::VisibilityContainer {
private:
  double const transition_time = 0.5;
  CCLayerGradient2* gradient;
public:
  
  static Toolbar* create() {
    $create_class(Toolbar, init);
  }
  bool init() override;

  /// The docstring for the HoverInterceptor in the osu! repo is this:

  /// <summary>
  /// Whenever the mouse cursor is within the bounds of the toolbar, we want the background gradient to show, for toolbar button descriptions to be legible.
  /// Unfortunately we also need to ensure that the toolbar buttons handle hover, to prevent the possibility of multiple descriptions being shown
  /// due to hover events passing through multiple buttons.
  /// This drawable is a workaround, that when placed front-most in the toolbar, allows to see whether hover events have been propagated through it without handling them.
  /// </summary>

  /// Genuinely speaking, it would be better to handle inputs in the visible boundary
  /// (like excluding the padding portion), and it will solve the problem.
  
  void show();
  void hide();
};
GDL_NS_END
