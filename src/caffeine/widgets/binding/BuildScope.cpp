#include <gdlazer/caffeine/widgets/binding/BuildScope.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>
#include <stdexcept>

namespace caffeine {
  
void BuildScope::tryRebuild(RefNauseam<Element> element) {
  element->rebuild();
}

void BuildScope::scheduleBuildFor(Element* element) {
  if (element->m_inDirtyList) {
    geode::log::warn("{} already in a dirty list", element->toString());
    return;
  }
  m_dirtyElements.push_back(element);
  element->m_inDirtyList = true;
  if (m_dirtyElementsNeedsResorting != 0) {
    throw std::runtime_error("");
    m_dirtyElementsNeedsResorting = 3;
  }
};

void BuildScope::flushDirtyElements() {
  massert(m_dirtyElementsNeedsResorting == 0, "_flushDirtyElements must be non-reentrant");

  m_dirtyElements.sort(Element::_sort);
  m_dirtyElementsNeedsResorting = 2; // 10
  geode::log::debug("[BuildScope::flushDirtyElements]:");
  auto prevIter = m_dirtyElements.end();
  for (auto i = m_dirtyElements.begin(); i != m_dirtyElements.end(); i=_seekDirtyElementAfterIter(i)) {
    if (prevIter == i) {
      geode::log::warn("Iterator seemingly doesn't move.");
      break;
    }
    auto& e = *i;
    auto elementScope = e->getBuildScope();
    geode::log::debug("[BuildScope::flushDirtyElements]: exist: {} isThis: {}",elementScope!=nullptr,elementScope.get() == this);
    if (elementScope && elementScope.get() == this) {
      tryRebuild(e);
    }
  }
  for (auto& i : m_dirtyElements) {
    auto elementScope = i->getBuildScope();
    if (elementScope && elementScope.get() == this) {
      i->m_inDirtyList = false;
    }
  }
  m_dirtyElementsNeedsResorting = 0;
  m_dirtyElements.clear();
}

}
