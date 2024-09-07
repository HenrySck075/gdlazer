#pragma once

// uhh
// the uhh

#include <Geode/Geode.hpp>
#include "Container.hpp"

using namespace geode::prelude;

class ContainerLayout : public Layout {
protected:
    CCArray* getNodesToPosition(CCNode* on) const {
        auto arr = CCArray::create();
        for (auto child : CCArrayExt<CCNode*>(on->getChildren())) {
            if ((!m_ignoreInvisibleChildren || child->isVisible())) {
                arr->addObject(child);
            }
        }
        return arr;
    }; 
};
