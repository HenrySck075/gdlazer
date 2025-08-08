#pragma once

#include "tree/Container.hpp"
#include <Geode/cocos/include/cocos2d.h>

/// The node that actually renders the tree
class TheEverythingApp : public cocos2d::CCNode {
    ContainerPtr m_idk = new Container{
        .children = {
            // random data idk
            // bro be serious
            new Container {
                .rect = RectangleF {0, 0, 100, 100},
                .padding = RectangleF {10, 10, 80, 80},
                .backgroundColor = Color4 {1.0f, 0.0f, 0.0f, 1.0f},
            },
            new Container {
                .rect = RectangleF {100, 100, 200, 200},
                .padding = RectangleF {20, 20, 160, 160},
                .backgroundColor = Color4 {0.0f, 1.0f, 0.0f, 1.0f},
            }
        }
    };
public:
    void draw() override;
};