#include "BeatmapCarousel.hpp"
#include "../../../framework/graphics/containers/ScrollableContainer.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"

bool BeatmapCarousel::init(GJLevelList* list) {
    if (!Container::init()) return false;
    auto body = FillFlowContainer::create(FillDirection::Vertical);
    body->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    for (auto* level : CCArrayExt<GJGameLevel*>(list->getListLevelsArray(CCArray::create()))) {
        body->addChild(DrawableCarouselBeatmap::create(level));
    }
    auto scroll = ScrollableContainer::create(body);
    scroll->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    addListener(CarouselItemSelect::eventname, [this](NodeEvent* e){
        if (currentItem) currentItem->deselect();
        if (e->target()) {
            currentItem = static_cast<DrawableCarouselBeatmap*>(e->target());
        }
        e->preventDefault();
    });
    addChild(scroll);
    return true;
}