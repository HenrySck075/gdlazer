#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/ScrollContainer.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../utils.hpp"
#include "../../../frameworks/bindables/Bindable.hpp"

GDF_NS_START
class SectionsContainer : public ScrollContainer {
private:
  FillFlowContainer* m_contentsContainer;
protected:
  Bindable<Container*> m_currentSection;
public:
  bool init();
  $default_create(SectionsContainer);

  void addChild(CCNode* node);
  virtual void onSectionSelect(Container* new_);
};
GDF_NS_END
