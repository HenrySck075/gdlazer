#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include "../../macro.h"

GDL_NS_START
/// Get the incoming scene from an immediate pushScene/replaceScene calls made in `dispatcher`.
///
/// The scene is retained until a next call is made.
cocos2d::CCScene* captureIncomingScene(std::function<void()> dispatcher);
GDL_NS_END