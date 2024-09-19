#pragma once

#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../framework/graphics/containers/ScrollableContainer.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"
#include "../../../utils.hpp"
#include "../../../framework/utils/Reactive.hpp"

class SectionsContainer : public ScrollableContainer {
private:
    FillFlowContainer* contentsContainer;
protected:
    Reactive<Container*> currentSection;
public:
    bool init();
    default_create(SectionsContainer);

    void addChild(CCNode* node);
    virtual void onSectionSelect(Container* old, Container* new_);
};
