#include <gdlazer/caffeine/widgets/binding/BuildOwner.hpp>
#include <gdlazer/caffeine/widgets/framework/Element.hpp>
#include <gdlazer/caffeine/widgets/binding/BuildScope.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>


namespace caffeine {

BuildOwner::BuildOwner(decltype(m_onBuildScheduled) onBuildScheduled) 
  : m_onBuildScheduled(onBuildScheduled), m_buildScope(new BuildScope(onBuildScheduled)) {}


void BuildOwner::_registerGlobalKey(
  GlobalKeyU* key,
  Element* element
) {
  // Store mapping from raw pointer to RefNauseam<Element>
  m_globalKeyRegistry[key] = element;
  // Also store the element in the key for quick access
  key->m_currentContext = dynamic_cast<BuildContext*>(element);
}

void BuildOwner::_unregisterGlobalKey(
  GlobalKeyU* key,
  Element* element
) {
  // Only remove if it's the same element (check pointer equality)
  auto it = m_globalKeyRegistry.find(key);
  if (it != m_globalKeyRegistry.end() && it->second.get() == element) {
    m_globalKeyRegistry.erase(it);
  }
  key->m_currentContext = nullptr;
}

void BuildOwner::scheduleBuildFor(Element* element) {
  auto scope = element->getBuildScope();
  massert(scope, "Element must have an assigned BuildScope");
  if (!m_scheduledFlushDirtyElement && m_onBuildScheduled.has_value()) {
    m_scheduledFlushDirtyElement = true;
    m_onBuildScheduled.value()();
  }
  scope->scheduleBuildFor(element);
}



void BuildOwner::buildScope(Element* context, std::optional<VoidCallback> callback) {
  auto scope = context->getBuildScope();
  massert(scope, "Element must have an assigned BuildScope");
  geode::log::debug("[BuildOwner::buildScope]: {}", !callback.has_value() && scope->m_dirtyElements.empty());
  
  if (!callback.has_value() && scope->m_dirtyElements.empty()) return;
  assert(dm_stateLockLevel >= 0);
  assert(!dm_building);
  #ifdef GDF_DEBUG
    dm_stateLockLevel++;
    dm_building = true;
  #endif
  /*try {*/
    m_scheduledFlushDirtyElement = true;
    scope->m_building = true;
    if (callback.has_value()) callback.value()();
    scope->flushDirtyElements();
  /*} catch (std::exception& s) {}*/
  scope->m_building = false;
  m_scheduledFlushDirtyElement = false;
  assert(dm_building);
  #ifdef GDF_DEBUG
    dm_building = false;
    dm_stateLockLevel--;
  #endif
  assert(dm_stateLockLevel >= 0);
}

void BuildOwner::finalizeTree() {
#ifdef GDF_DEBUG
  dm_stateLockLevel++;
#endif
  m_inactiveElements._unmountAll();
}
}
