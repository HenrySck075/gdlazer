#include "Container.hpp"
#include "../CCPointExtensions.hpp"
#include "../../input/events/MouseEvent.hpp"

/**
 * RoundedRectDrawNode
 */
void balls::drawCircle(const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const ccColor4F &color) {
  const float coef = 2.0f * (float)M_PI/segments;
  
  CCPoint *vertices = new (std::nothrow) CCPoint[segments+2];
  if( ! vertices )
    return;
  
  for(unsigned int i = 0;i <= segments; i++) {
    float rads = i*coef;
    float j = radius * cosf(rads + angle) * scaleX + center.x;
    float k = radius * sinf(rads + angle) * scaleY + center.y;
    
    vertices[i].x = j;
    vertices[i].y = k;
  }
  if(drawLineToCenter)
  {
    vertices[segments+1].x = center.x;
    vertices[segments+1].y = center.y;
    drawPolygon(vertices, segments+2, color, 0, color);
  }
  else
    drawPolygon(vertices, segments+1, color, 0, color);
  
  CC_SAFE_DELETE_ARRAY(vertices);
}
void balls::drawRoundedRect() {
  // just dont do anything
  if (m_obContentSize == CCSize{0,0} || !stencilEnabled()) return;
  // determine whether radius percentage should follow the width or the height
  auto useWidth = m_obContentSize.width < m_obContentSize.height;
  float radius = (m_radius/100) * (
    useWidth ? m_obContentSize.width : m_obContentSize.height
  );

  CCRectExtra innerRect = {
    radius,
    radius,
    m_obContentSize - CCSize(radius * 2, radius * 2)
  };
  // location, radius, fill, outline size, outline, amount of triangles i suppose
  ccColor4F color = {0.f,0.f,200.f/255.f,1.f};
  #define drawRectConfig color, 0, color
  #define drawCircConfig radius, 0, 30, true, color
  if (radius > 0) {
    //(const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const ccColor4F &color)

    log::debug(
      "[RoundedBorderStencil]:\n{} | {} \n{} | {} | {} | {}", 
      innerRect.origin, innerRect.size,
      innerRect.getMinX(),innerRect.getMinY(),innerRect.getMaxX(),innerRect.getMaxY()
    );

    CCPoint tl {innerRect.getMinX(),innerRect.getMaxY()};
    CCPoint tr {innerRect.getMaxX(),innerRect.getMaxY()};
    CCPoint bl {innerRect.getMinX(),innerRect.getMinY()};
    CCPoint br {innerRect.getMaxX(),innerRect.getMinY()};

    drawCircle(tl, drawCircConfig);
    drawCircle(tr, drawCircConfig);
    drawCircle(bl, drawCircConfig);
    drawCircle(br, drawCircConfig);

    drawRect(innerRect, drawRectConfig);
    
    //top
    drawRect({radius,innerRect.getMaxY(),innerRect.getMaxX()-radius,radius},drawRectConfig);
    //bottom
    drawRect({radius,0,innerRect.getMaxX()-radius,radius},drawRectConfig);
    //left
    drawRect({0,radius,radius,innerRect.getMaxY()-radius},drawRectConfig);
    //right
    drawRect({innerRect.getMaxX(),radius,radius,innerRect.getMaxY()-radius},drawRectConfig);
  }
  else {
    // do it fast
    // actually the stencil is disabled at this point so dont even bother doing this
    // drawRect(innerRect, peakDesign);
  }
  #undef peakDesign
  #undef drawCircConfig
  #undef drawRectConfig
}

bool balls::stencilEnabled() { 
  //if (static_cast<CCBool*>(getUserObject("stencil"_spr)))
  return
  #ifndef GEODE_IS_ANDROID
  m_radius!=0
  #else
  // stencil code in the repo does not work on android
  // so this is a temporary return until that issue is resolved 
  // (by getting the re code from libcocos2dcpp.so)
  false 
  #endif
  ; 
}

bool balls::init(float rad) {
  if (!CCDrawNode::init())
    return false;
  setOpacity(255);
  setRadius(rad);
  return true;
}

void balls::setContentSize(const CCSize &size) {
  bool j = size != m_obContentSize;
  if (j) clear();
  CCDrawNode::setContentSize(size);
  if (j) drawRoundedRect();
}

void balls::setRadius(float rad) {
  if (rad == m_radius)
  return;
  m_radius = std::max(rad, 0.f);
  clear();
  drawRoundedRect();
}

float balls::getRadius() { return m_radius; };

/**
 * InputHandlerImpl
 */
void Container::initHandler() {
  addListener("mouseEvent",[this](NodeEvent* e) {
    auto event = static_cast<MouseEvent*>(e);
    auto type = event->eventType;
    //if (type == MouseEventType::Move) event->logging(true);
    if (!(m_hoverEnabled && isRunning())) return;
    switch (type) {
      case MouseEventType::Move:
        onMouseMove(event);
        break;
      case MouseEventType::Enter:
        m_entered = true;
        onMouseEnter();
        break;
      case MouseEventType::Exit:
        m_entered = false;
        onMouseExit();
        break;
      case MouseEventType::MouseUp:
        if (m_holding) {
          onMouseUp(event);
          if (dragEnabled() && currentDragEvent) {
            onDragEnd(currentDragEvent);
            currentDragEvent->release();
            currentDragEvent = nullptr;
          }
          m_holding = false;
        }
        break;
      case MouseEventType::MouseDown:
        if (m_entered) {
          onMouseDown(event);
          mouseDownPos = event->position;
          m_holding = true;
        }
        break;
      case MouseEventType::MouseScroll:
        if (m_entered) {
          onMouseScroll(event);
        }
        return;
      case MouseEventType::Click:
        onClick(event);
        event->preventDefault();
    }
    int type2 = (int)type;
    if (type == MouseEventType::MouseUp && m_entered) { //&& boundingBoxFromContentSize(this).containsPoint(mouseDownPos)) {
      type2 = MouseEventType::Click;
    }
    else if (type == MouseEventType::Move) {
      auto bound = boundingBoxFromContentSize(this);
      bool containsCursor = bound.containsPoint(event->position);
      if (
        containsCursor
        && 
        !m_entered
      ) type2 = MouseEventType::Enter;
      if (
        !containsCursor
        && 
        m_entered
      ) type2 = MouseEventType::Exit;
    }
    if (type2 != (int)type) {
      EventTarget::dispatchEvent(new MouseEvent((MouseEventType)type2, event->position));
    }

    if (dragEnabled() && type == MouseEventType::Move && m_entered && m_holding) {
      if (CCPointExtensions::distance(
        mouseDownPos.equals({0,0}) ? event->position : mouseDownPos, 
        event->position
      ) > clickDragDistance) {
        if (!currentDragEvent) {
          currentDragEvent = new MouseDragEvent(mouseDownPos, event->position);
          currentDragEvent->retain();
          onDragStart(currentDragEvent);
        } else {
          currentDragEvent->current = event->position;
          onDrag(currentDragEvent);
        }
      }
    }
    return;
  });
}

void Container::setClickEnabled(bool e) { m_clickEnabled = e; }

bool Container::getClickEnabled() { return m_clickEnabled; }

void Container::setHoverEnabled(bool state) { 
  m_hoverEnabled = state; 
  if (!m_hoverEnabled) {onMouseExit();}
  else {
#ifdef GEODE_IS_WINDOWS
    auto director = CCDirector::sharedDirector();
    auto pos = director->getOpenGLView()->getMousePosition();
    auto realSize = director->getOpenGLView()->getDisplaySize();
    auto winSize = director->getWinSize();

    auto p = CCPoint(
      pos.x / realSize.width * winSize.width, 
      ((realSize.height-pos.y) / realSize.height * winSize.height)
    );

    dispatchEvent(new MouseEvent(MouseEventType::Move, p));
#endif
  }
}

bool Container::getHoverEnabled() { return m_hoverEnabled; }


/**
 * Container
 */

std::map<Anchor, std::pair<ah,av>> Container::m_anchors = {
  {Anchor::TopLeft, std::make_pair(ah::Left, av::Top)},
  {Anchor::Top, std::make_pair(ah::Center, av::Top)},
  {Anchor::TopRight, std::make_pair(ah::Right, av::Top)},

  {Anchor::Left, std::make_pair(ah::Left, av::Center)},
  {Anchor::Center, std::make_pair(ah::Center, av::Center)},
  {Anchor::Right, std::make_pair(ah::Right, av::Center)},

  {Anchor::BottomLeft, std::make_pair(ah::Left, av::Bottom)},
  {Anchor::Bottom, std::make_pair(ah::Center, av::Bottom)},
  {Anchor::BottomRight, std::make_pair(ah::Right, av::Bottom)} 
};

std::map<Anchor, std::string> Container::m_anchorDebugLabel = {
  {Anchor::TopLeft, "top left"},
  {Anchor::Top, "top center"},
  {Anchor::TopRight, "top right"},

  {Anchor::Left, "center left"},
  {Anchor::Center, "center*2"},
  {Anchor::Right, "center right"},

  {Anchor::BottomLeft, "bottom left"},
  {Anchor::Bottom, "bottom center"},
  {Anchor::BottomRight, "bottom right"} 
};

std::string Container::getUnitLabel(Unit unit) {
  std::string l;
  switch (unit) {
    case Unit::OpenGL: 
      l = "OpenGL Points";
      break;
    case Unit::UIKit: 
      l = "UIKit Points";
      break;
    case Unit::Percent: 
      l = "Parent's percentage";
      break;
    case Unit::Viewport: 
      l = "Viewport";
      break;
  }
  return l;
}

bool Container::init() {
  m_roundedBorderStencil = balls::create();
  //m_roundedBorderStencil->setRadius(8);
  if (!CCClippingLayer::init({255,255,255,0},m_roundedBorderStencil)) return false;

  addListener("nodeLayoutUpdate", [this](NodeEvent*j){
    onLayoutUpdate(static_cast<NodeLayoutUpdate*>(j));
    checkConstraints();
    m_roundedBorderStencil->setContentSize(CCNode::getContentSize());
  });
  initHandler();
  ignoreAnchorPointForPosition(false);
  setAnchorPoint({0,0});
  updateAnchorLabel();
  updateSizeUnitLabel();
  updatePositionUnitLabel();
  //setUserObject("geode.devtools/useRealAttributes", CCBool::create(true));
  return true;
}

Container* Container::create() {
  $create_class(Container, init);
}

bool Container::dispatchEvent(NodeEvent* event) {
  event->retain();
  if (event->m_log && !m_ignoreLogging) log::debug("[{} | EventTarget]: Dispatching {}", getNodeName(this), event->m_eventName);
  auto ret = EventTarget::dispatchEvent(event);
  /*
  colorBg->setColor(ccc3(255,171,15));
  colorBg->setOpacity(150);
  colorBg->runAction(CCFadeTo::create(0.5,0));
  */
  if (!ret) return false;
  if (event->m_stopPropagate) {
    event->m_stopPropagate = false;
    // according to the parent this is not really a cancel event, but if you want 
    // you can check for m_stopPropagate like i did here
    return true;
  }
  // workaround to prevent one heck of a call stack
  //
  // TODO: use some sort of a global event listeners mapper
  // so we can just do everything in 1 call
  switch (event->m_dispatchingFlow) {
    case DispatchingFlow::Up:
      if (auto p = dynamic_cast<EventTarget*>(m_pParent)) return p->dispatchEvent(event);
    case DispatchingFlow::Down:
      return dispatchToChild(event);
  }
  event->release();

  return true;
}

bool Container::dispatchToChildInList(NodeEvent* event, CCArray* children) {
  event->m_dispatchingFlow = DispatchingFlow::Down;
  auto the = CCArrayExt<CCNode*>(children);

  auto rev_iter_start = std::reverse_iterator(the.end());
  auto rev_iter_end = std::reverse_iterator(the.begin());
  
  while (rev_iter_start != rev_iter_end) {
    auto node = *rev_iter_start++;
  
    if (node == this) {
      log::warn("[Container]: Stack overflow prevented: {} contains itself.", this);
      continue;
    }
    if (auto target = typeinfo_cast<EventTarget*>(node)) {
      if (!target->dispatchEvent(event)) return false;
    } else {
      // recursively finding a valid EventTarget
      if (!dispatchToChildInList(event,node->getChildren())) return false;
    }
  }
  
  return true;
}

bool Container::dispatchToChild(NodeEvent* event) {
  return dispatchToChildInList(event, m_pChildren);
};

CCSize const& Container::contentSizeWithPadding(CCSize const& size, Vector4 padding) {
  return {
    size.width - processUnit(padding.l + padding.r, Unit::UIKit, true),
    size.height - processUnit(padding.t + padding.d, Unit::UIKit, false)
  };
}

void Container::onLayoutUpdate(NodeLayoutUpdate* e) {
  if (m_pParent == nullptr || !isRunning()) return;
  CCPoint oldP = CCNode::getPosition();
  CCPoint resP = {0,0};
  auto anchor = m_anchors[m_anchor];
  auto openglPos = CCPoint(
    processUnit(m_position.x,m_positionUnit.first,true),
    processUnit(m_position.y,m_positionUnit.second,false)
  );

  auto parentSize = m_pParent->CCNode::getContentSize();

  // getContentWidth/Height get its values from getContentSize so like
  switch(anchor.first) {
    case ah::Left:
      resP.x = openglPos.x; 
      break;
    case ah::Center:
      resP.x = parentSize.width/2+openglPos.x; 
      break;
    case ah::Right:
      resP.x = parentSize.width-openglPos.x; 
      break;
  };
  switch(anchor.second) {
    case av::Top:
      resP.y = parentSize.height-openglPos.y; 
      break;
    case av::Center:
      resP.y = parentSize.height/2-openglPos.y; 
      break;
    case av::Bottom:
      resP.y = openglPos.y; 
      break;
  }
  //bool nothingHappens = false;
  //if (resP.equals(oldP) && !resetContentSize()) nothingHappens = true;
  //if (nothingHappens) e->stopPropagation();
  resetContentSize();
  CCNode::setPosition(resP+CCPoint{processUnit(m_padding.t, Unit::UIKit, true),processUnit(m_padding.l, Unit::UIKit, false)});
};

float Container::processUnit(float value, Unit unit, bool width) {
  if (m_pParent == nullptr && unit == Unit::Percent) {
    // does not have a parent
    return value;
  }
  auto ccsize = [](CCSize size, bool w) {
    return w ? size.width : size.height;
  };
  switch (unit) {
  case Unit::OpenGL:
    return value;
  case Unit::UIKit:
    return 
      value 
      / 
      (
        ccsize(CCDirector::sharedDirector()->getOpenGLView()->getFrameSize(),width)
        /
        ccsize(CCDirector::sharedDirector()->getWinSize(),width)
      )
      * 
      Mod::get()->template getSettingValue<int64_t>("uiscale");
  case Unit::Viewport:
    return value * (width ? CCDirector::sharedDirector()->getWinSize().width : CCDirector::sharedDirector()->getWinSize().height);
  case Unit::Percent:
    auto m_pParentContainer = dynamic_cast<Container*>(m_pParent);
    return 
    (value / 100) 
    * 
    ccsize(
      contentSizeWithPadding(
        m_pParent->CCNode::getContentSize(),
        m_pParentContainer ? m_pParentContainer->getPadding() : Vector4{0}
      ),
      width
    )
    ;
  };
}

void Container::setSizeConstraints(CCSize const& minSize, CCSize const& maxSize) {
  // who did this
  if (minSize.width < maxSize.width && minSize.height < maxSize.height) {
    log::error("Monika: you got it the wrong way.");
    return;
  }

  minimumSize = CCSize{
    processUnit(minSize.width,m_sizeUnit.first,true),
    processUnit(minSize.height,m_sizeUnit.second,false)
  };
  maximumSize = CCSize{
    processUnit(maxSize.width,m_sizeUnit.first,true),
    processUnit(maxSize.height,m_sizeUnit.second,false)
  };
  checkConstraints();
}

void Container::checkConstraints() {
  auto currentSize = CCNode::getContentSize();
  bool d = false;
  if (currentSize.width < minimumSize.width) {
    currentSize.width = minimumSize.width;
    d = true;
  }
  if (currentSize.height < minimumSize.height) {
    currentSize.height = minimumSize.height;
    d = true;
  }
  if (!maximumSize.equals(CCSize(0,0))) {
    if (currentSize.width > maximumSize.width) {
      currentSize.width = maximumSize.width;
      d = true;
    }
    if (currentSize.height > maximumSize.height) {
      currentSize.height = maximumSize.height;
      d = true;
    }
  }
  if (d) {
    // mark the node as dirty so cocos2d will actually attempt to do something with that (i suppose)
    CCLayerColor::setContentSize(currentSize);
    // todo: remove this
    dispatchToChild(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
  }
}

bool Container::resetContentSize() {
  auto newS = CCSize(
    processUnit(m_size.width, m_sizeUnit.first, true),
    processUnit(m_size.height,m_sizeUnit.second, false)
  );
  if (newS.equals(CCNode::getContentSize())) return false;
  CCLayerColor::setContentSize(newS);
  // checkConstraints();
  m_sizeP = m_size;
  return true;
}
/*
bool ContainerNodeWrapper::init(CCNode* node)  {
  if (!typeinfo_cast<Container*>(node)) {
    log::error("[ContainerNodeWrapper]: The node passed does not meet the candidate to be a node: it is a {}", node);
    return false;
  }
  m_node = node;
  Container::init();
  //log::debug("[ContainerNodeWrapper]: {}", node);
  addChild(m_node);
  setContentSize(node->getContentSize());
  setPosition(node->getPosition());
  setAnchorPoint(node->getAnchorPoint());
  node->setPosition({0,0});
  node->setAnchorPoint({0,0});
  return true;
}

void ContainerNodeWrapper::dispatchToChild(NodeEvent* event) {
  CCObject* obj;
  log::debug("[ContainerNodeWrapper]: {} {}", this, m_node);
  auto c = m_node->getChildren();
  CCARRAY_FOREACH(c, obj) {
    if (auto node = typeinfo_cast<Container*>(obj)) {
      node->dispatchEvent(event);
    }
  }
};
*/
void Container::updateAnchorLabel() {
  setUserObject("of/anchor", CCString::create(m_anchorDebugLabel[m_anchor]));
}

void Container::updateSizeUnitLabel() {
  setUserObject("of/su",
    CCString::create(fmt::format("W: {} | H: {}", getUnitLabel(m_sizeUnit.first), getUnitLabel(m_sizeUnit.second)))
  );
}

void Container::updatePositionUnitLabel() {
  setUserObject("of/pu",
    CCString::create("X: " + getUnitLabel(m_positionUnit.first) +
              " | "
              "Y: " +
              getUnitLabel(m_positionUnit.second)));
}

#include <csignal>
void Container::updateColor() {
  //"C_Cpp.default.cppStandard": ""
  auto goog = static_cast<CCBool*>(getUserObject("breakhere"));
  if (goog && goog->getValue()) {
    // Generate an interrupt
    #ifdef GEODE_IS_WINDOWS
    DebugBreak();
    #else
    std::raise(SIGTRAP);
    #endif
  };
  for (unsigned int i = 0; i < 4; i++) {
    m_pSquareColors[i].r = m_color4.r / 255.f;
    m_pSquareColors[i].g = m_color4.g / 255.f;
    m_pSquareColors[i].b = m_color4.b / 255.f;
    /** TODO: 
     * currently using _realOpacity because _displayedOpacity somehow being set to 0.
     * diagnose the issue so opacity cascading can work normally.*/
    m_pSquareColors[i].a = (m_color4.a * (_realOpacity / 255.f)) / 255;
  }
}

void Container::setColor(const ccColor3B& color) { 
  setColor(Color4{color});
}

void Container::setColor(const Color4& color) {
  m_color4 = color;
  _displayedColor = _realColor = m_color4;
  updateColor();
} 

void Container::setParent(CCNode *parent) {
  CCLayer::setParent(parent);
  dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::All));
};
void Container::onEnter() {
  CCLayerColor::onEnter();
  dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::All));
}

