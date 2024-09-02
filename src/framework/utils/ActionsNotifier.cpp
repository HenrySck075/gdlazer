#include "ActionsNotifier.hpp"


#include <Geode/modify/CCActionManager.hpp>
struct hook5 : geode::Modify<hook5, CCActionManager> {
    void removeAction(CCAction *pAction) {
        if (!pAction) return;

        if (pAction->isDone()) ActionFinished(pAction).post();
        CCActionManager::removeAction(pAction);
    }
};