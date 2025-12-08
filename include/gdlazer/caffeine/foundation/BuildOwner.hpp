#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include <unordered_map>
#include "types.hpp"
#include "Element.hpp"
#include "Widget.hpp"

class BuildOwner {
  _InactiveElements m_inactiveElements;
  std::optional<VoidCallback> onBuildScheduled;
  friend class Element;


  std::unordered_map<std::shared_ptr<GlobalKeyU>, std::shared_ptr<Element>> m_globalKeyRegistry;

  void _registerGlobalKey(
    shared_ptr_ctor<GlobalKeyU> key,
    shared_ptr_ctor<Element> element
  ) {
    m_globalKeyRegistry[key] = element;
  }
  void _unregisterGlobalKey(
    shared_ptr_ctor<GlobalKeyU> key,
    shared_ptr_ctor<Element> element
  ) {
    auto idk = m_globalKeyRegistry.find(key);
    if (idk != m_globalKeyRegistry.end() && idk->second == element) {
      m_globalKeyRegistry.erase(idk);
    }
  }

#ifdef GDF_DEBUG
  bool dm_building = false;
  int dm_stateLockLevel = 0;
#endif
public:
  BuildOwner() = delete;
  BuildOwner(decltype(onBuildScheduled) onBuildScheduled) : onBuildScheduled(onBuildScheduled) {}

  void scheduleBuildFor(shared_ptr_ctor<Element> element);
  void buildScope(shared_ptr_ctor<Element> context, std::optional<VoidCallback> callback);
};
