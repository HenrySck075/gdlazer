#include "NowPlayingOverlay.hpp"

bool NowPlayingOverlay::init() {
    if (!OsuOverlayContainer::init()) return false;
    main->setAnchor(Anchor::TopRight);
    main->setPadding({3});
    main->setRadius(8);
    return true;
}