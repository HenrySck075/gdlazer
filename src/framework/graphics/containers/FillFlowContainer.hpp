#pragma once

#include "Container.hpp"
#include "ContainerLayout.hpp"

enum class FillDirection {
  Vertical,
  Horizontal
};


// wrapper container around geode's layout system
class FillFlowContainer : public Container {
private:
  FillDirection m_direction;
  void updateChildPosition();
public:
  static FillFlowContainer* create(FillDirection dir) {
    $create_class(FillFlowContainer, init, dir);
  }
  void addChild(CCNode* node);
  void setFillDirection(FillDirection dir);
  FillDirection getFillDirection() {return m_direction;}
  bool init(FillDirection dir);
  /*
  void addChild(CCNode* child) {
    if (auto c = typeinfo_cast<Container*>(child)) {
      Container::addChild(child);
      updateChildPosition();
    }
  };
  */

  void setContentSize(CCSize const& size) override {
  }
  void updateLayout(bool googoo = false) {
    CCLayer::updateLayout(googoo);
  };

  bool dispatchEvent(NodeEvent* event) override {
    bool ret = Container::dispatchEvent(event);
    if (ret) {
      if (event->eventName() == "nodeLayoutUpdate") {
        auto e = static_cast<NodeLayoutUpdate*>(event);
        if (e->type==NodeLayoutUpdateType::Size || e->type==NodeLayoutUpdateType::All) {
          updateLayout();
        }
      };
    }
    return ret;
  };
};
