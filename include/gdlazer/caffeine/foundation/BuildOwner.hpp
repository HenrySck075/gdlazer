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
  std::shared_ptr<BuildScope> m_buildScope;

  std::unordered_map<std::shared_ptr<GlobalKeyU>, std::shared_ptr<Element>> m_globalKeyRegistry;

  void _registerGlobalKey(
    GlobalKeyU* key,
    Element* element
  );
  void _unregisterGlobalKey(
    GlobalKeyU* key,
    Element* element
  );

#ifdef GDF_DEBUG
  bool dm_building = false;
  int dm_stateLockLevel = 0;
#endif
public:
  BuildOwner() = delete;
  BuildOwner(decltype(onBuildScheduled) onBuildScheduled);

  void scheduleBuildFor(Element* element);
  void buildScope(Element* context, std::optional<VoidCallback> callback);
  std::shared_ptr<BuildScope> getRootBuildScope() { return m_buildScope; }
};
