#include <gdlazer/caffeine/foundation/BuildOwner.hpp>
#include <gdlazer/caffeine/foundation/Element.hpp>
#include <gdlazer/caffeine/foundation/BuildScope.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>

void BuildOwner::scheduleBuildFor(shared_ptr_ctor<Element> element) {
  element->getBuildScope()->scheduleBuildFor(element);
}



void BuildOwner::buildScope(shared_ptr_ctor<Element> context, std::optional<VoidCallback> callback) {
  auto scope = context->getBuildScope();
  if (!callback.has_value() && scope->m_dirtyElements.empty()) return;
  assert(dm_stateLockLevel >= 0);
  assert(!dm_building);
  #ifdef GDF_DEBUG
    dm_stateLockLevel++;
    dm_building = true;
  #endif
  try {
    scope->m_building = true;
    callback.value()();
    scope->flushDirtyElements();
  } catch (std::exception& s) {}
  scope->m_building = false;
  assert(dm_building);
  #ifdef GDF_DEBUG
    dm_building = false;
    dm_stateLockLevel--;
  #endif
  assert(dm_stateLockLevel >= 0);
}
