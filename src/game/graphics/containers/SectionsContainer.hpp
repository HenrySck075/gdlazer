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
  bool sectionClickingShenanigans(MouseEvent* event);
public:
  bool init();
  $defaultCreate(SectionsContainer);

  void addSection(Container* section);
  virtual void onSectionSelect(Container* new_);

  void updateLayout();
};
GDF_NS_END
