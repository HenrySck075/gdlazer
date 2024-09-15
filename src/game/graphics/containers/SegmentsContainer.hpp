#pragma once

#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../framework/graphics/containers/ScrollableContainer.hpp"
#include "../../../utils.hpp"

class SegmentsContainer : public Container {
protected:
    ScrollableContainer* contents;
    Container* selectedSection;
public:
    bool init();
    default_create(SegmentsContainer);
};