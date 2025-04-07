#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/ScrollableContainer.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../utils.hpp"
#include "../../../frameworks/utils/Reactive.hpp"

class SectionsContainer : public ScrollableContainer {
private:
  FillFlowContainer* contentsContainer;
protected:
  Reactive<Container*> currentSection;
public:
  bool init();
  $default_create(SectionsContainer);

  void addChild(CCNode* node);
  virtual void onSectionSelect(Container* old, Container* new_);
};
