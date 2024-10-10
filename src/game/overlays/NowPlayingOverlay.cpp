#include "NowPlayingOverlay.hpp"

bool NowPlayingOverlay::init() {
    if (!OsuOverlayContainer::init()) return false;
    main->setAnchor(Anchor::TopRight);
    main->setPadding({3});
    main->setRadius(8);
    main->setContentSizeWithUnit({player_width,player_height}, Unit::UIKit, Unit::UIKit);
    return true;
}
