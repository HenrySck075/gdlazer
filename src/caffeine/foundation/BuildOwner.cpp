#include <gdlazer/caffeine/foundation/BuildOwner.hpp>
#include <gdlazer/caffeine/foundation/Element.hpp>
#include <gdlazer/caffeine/foundation/BuildScope.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>

BuildOwner::BuildOwner(decltype(onBuildScheduled) onBuildScheduled) 
  : onBuildScheduled(onBuildScheduled), m_buildScope(std::make_shared<BuildScope>(onBuildScheduled)) {}


void BuildOwner::_registerGlobalKey(
  GlobalKeyU* key,
  Element* element
) {
  m_globalKeyRegistry[std::shared_ptr<GlobalKeyU>(key)] = element->shared_from_this();
  key->m_currentContext = std::static_pointer_cast<BuildContext>(element->shared_from_this());
}

void BuildOwner::_unregisterGlobalKey(
  GlobalKeyU* key,
  Element* element
) {
  auto it = m_globalKeyRegistry.find(std::shared_ptr<GlobalKeyU>(key));
  if (it != m_globalKeyRegistry.end()) {
    m_globalKeyRegistry.erase(it);
  }
  key->m_currentContext = nullptr;
}

void BuildOwner::scheduleBuildFor(Element* element) {
  auto scope = element->getBuildScope();
  massert(scope, "Element must have an assigned BuildScope");
  scope->scheduleBuildFor(element);
}



void BuildOwner::buildScope(Element* context, std::optional<VoidCallback> callback) {
  auto scope = context->getBuildScope();
  massert(scope, "Element must have an assigned BuildScope");
  
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
