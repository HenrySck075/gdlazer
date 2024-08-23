#include "BeatmapCarousel.hpp"
#include "../../../framework/graphics/containers/ScrollableContainer.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"
#include "carousel/DrawableCarouselBeatmap.hpp"

bool BeatmapCarousel::init(GJLevelList* list) {
    if (!Container::init()) return false;
    auto body = FillFlowContainer::create(FillDirection::Vertical);
    for (auto* level : CCArrayExt<GJGameLevel*>(list->getListLevelsArray(CCArray::create()))) {
        body->addChild(DrawableCarouselBeatmap::create(level));
    }
    auto scroll = ScrollableContainer::create(body);
    scroll->setAnchor(Anchor::Right);
    addChild(scroll);
    return true;
}