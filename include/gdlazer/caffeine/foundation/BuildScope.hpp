#pragma once
#include "Element.hpp"
#include <cstdint>
#include <optional>
#include "types.hpp"

class BuildScope final {
protected:
  friend class BuildOwner;
  std::list<std::shared_ptr<Element>> m_dirtyElements;
  using DirtyElementsIter = decltype(m_dirtyElements)::iterator;
  // in bits
  // 00: null
  // 10: false
  // 11: true
  // why not lmao
  uint8_t m_dirtyElementsNeedsResorting = 0;
  bool m_building = false; friend class BuildOwner;
  std::optional<VoidCallback> m_scheduleRebuildCallback;

  void tryRebuild(std::shared_ptr<Element> element);
  void scheduleBuildFor(Element* element);
  void flushDirtyElements();
  inline void _seekDirtyElementAfterIter(DirtyElementsIter iter) {
    if (m_dirtyElementsNeedsResorting != 3) {
      ++iter;
      return;
    }
    m_dirtyElements.sort(Element::_sort);
    m_dirtyElementsNeedsResorting = 2;
    while (iter != m_dirtyElements.begin() && iter->get()->isDirty()) {
      // It is possible for previously dirty but inactive widgets to move right in the list.
      // We therefore have to move the index left in the list to account for this.
      // We don't know how many could have moved. However, we do know that the only possible
      // change to the list is that nodes that were previously to the left of the index have
      // now moved to be to the right of the right-most cleaned node, and we do know that
      // all the clean nodes were to the left of the index. So we move the index left
      // until just after the right-most clean node.
      iter--;
    }
  }
public:
  BuildScope(decltype(m_scheduleRebuildCallback) scheduleRebuild = std::nullopt) : m_scheduleRebuildCallback(scheduleRebuild) {};
  virtual ~BuildScope() = default;
};
