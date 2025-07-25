#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/ScrollContainer.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../utils.hpp"
#include "../../../frameworks/bindables/Bindable.hpp"

GDF_NS_START

class SectionsContainer : public ScrollContainer {
private:
  geode::Ref<FillFlowContainer> m_contentsContainer;
protected:
  Bindable<Container*> m_currentSection;
  bool sectionClickingShenanigans(MouseEvent* event);
  EventListener<MouseEvent> m_questionableCode;
public:
  bool init();
  $defaultCreate(SectionsContainer);

  void addSection(Container* section);
  virtual void onSectionSelect(Container* old, Container* new_);

  void onEnter() override;

  void updateLayout();
};
GDF_NS_END
