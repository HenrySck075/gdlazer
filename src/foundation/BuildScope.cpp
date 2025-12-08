#include <gdlazer/caffeine/foundation/BuildScope.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>

void BuildScope::tryRebuild(std::shared_ptr<Element> element) {
  element->rebuild();
}

void BuildScope::scheduleBuildFor(shared_ptr_ctor<Element> element) {
  if (element->m_inDirtyList) return;
  m_dirtyElements.push_back(element);
  element->m_inDirtyList = true;
  if (m_dirtyElementsNeedsResorting != 0) {
    m_dirtyElementsNeedsResorting = 3;
  }
};

void BuildScope::flushDirtyElements() {
  massert(m_dirtyElementsNeedsResorting == 0, "_flushDirtyElements must be non-reentrant");

  m_dirtyElements.sort(Element::_sort);
  m_dirtyElementsNeedsResorting = 2; // 10
  for (auto i = m_dirtyElements.begin(); i != m_dirtyElements.end(); _seekDirtyElementAfterIter(i)) {
    auto& e = *i;
    if (e->getBuildScope().get() == this) tryRebuild(e);
  }
  for (auto& i : m_dirtyElements) {
    if (i->getBuildScope().get() == this) {
      i->m_inDirtyList = false;
    }
  }
}
