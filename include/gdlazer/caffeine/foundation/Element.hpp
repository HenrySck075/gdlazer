#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <Geode/cocos/include/cocos2d.h>
#include <gdlazer/caffeine/foundation/utils/shared_ptr_2.hpp>
#include <gdlazer/caffeine/foundation/Key.hpp>

// Element
class BuildContext {};

class Widget;
class Element;
class BuildOwner;
class BuildScope;
using ElementVisitor = std::function<void(shared_ptr_ctor<Element>)>;
enum class _ElementLifecycle {
  /// The [Element] is created but has not yet been incorporated into the element
  /// tree.
  initial,

  /// The [Element] is incorporated into the Element tree, either via
  /// [Element.mount] or [Element.activate].
  active,

  /// The previously `active` [Element] is removed from the Element tree via
  /// [Element.deactivate].
  ///
  /// This [Element] may become `active` again if a parent reclaims it using
  /// a [GlobalKey], or `defunct` if no parent reclaims it at the end of the
  /// build phase.
  inactive,

  /// The [Element] encountered an unrecoverable error while being rebuilt when it
  /// was `active` or while being incorporated in the tree.
  ///
  /// This indicates the [Element]'s subtree is in an inconsistent state and must
  /// not be re-incorporated into the tree again.
  ///
  /// When an unrecoverable error is encountered, the framework calls
  /// [Element.deactivate] on this [Element] and sets its state to `failed`. This
  /// process is done on a best-effort basis and does not surface any additional
  /// errors.
  ///
  /// This is one of the two final stages of the element lifecycle and is not
  /// reversible. Reaching this state typically means that a widget implementation
  /// is throwing unhandled exceptions that need to be properly handled.
  failed,

  /// The [Element] is disposed and should not be interacted with.
  ///
  /// The [Element] must be `inactive` before transitioning into this state,
  /// and the state transition occurs in [BuildOwner.finalizeTree] which signals
  /// the end of the build phase.
  ///
  /// This is the final stage of the element lifecycle and is not reversible.
  defunct,
};


class _InactiveElements {
  friend class Element;
  std::list<std::shared_ptr<Element>> m_elements;
  bool m_locked = false;

  static void _unmount(std::shared_ptr<Element> element);
  void _unmountAll();

  void _deactivateRecursively(shared_ptr_ctor<Element> element);

public:
  void add(std::shared_ptr<Element> element);
  void remove(std::shared_ptr<Element> element);
};

class Element : public BuildContext {
  friend class _InactiveElements;
  std::shared_ptr<BuildScope> m_parentBuildScope;
protected:
  std::shared_ptr<BuildOwner> m_owner;
  std::shared_ptr<Element> m_parent; friend class RenderObjectElement;
  std::shared_ptr<Widget> m_widget;
  /// idk what do they mean by this their usage is confusing
  _ElementLifecycle m_lifecycleState = _ElementLifecycle::initial;
  bool m_dirty = 0;
  bool m_inDirtyList = false; friend class BuildScope;
  int m_depth = 0;

  void* m_slot = 0;

  std::shared_ptr<Element> _retakeInactiveElement(
    GlobalKeyU* key, Widget* widget
  );
public:
  static bool _sort(std::shared_ptr<Element> a, std::shared_ptr<Element> b) {
    if (int diff = a->m_depth - b->m_depth) return diff < 0;
    bool bDirty = b->m_dirty;
    if (a->m_dirty != bDirty) return bDirty;
    return true;
  }


  Element(shared_ptr_ctor<Widget> widget) : m_widget(widget) {}

  /// Get the render object (aka cocos node) at current (or below) element
  virtual cocos2d::CCNode *getRenderObject();

  void mount(shared_ptr_ctor<Element> parent, void* slot);
  /*
   *Transition from the "inactive" to the "defunct" lifecycle state.

Called when the framework determines that an inactive element will never be reactivated. At the end of each animation frame, the framework calls unmount on any remaining inactive elements, preventing inactive elements from remaining inactive for longer than a single animation frame.

After this function is called, the element will not be incorporated into the tree again.

Implementations of this method should end with a call to the inherited method.
   */
  void unmount();
  void markNeedsBuild();
  void rebuild();
  virtual void performRebuild();

  void update(shared_ptr_ctor<Widget> newWidget);
  std::shared_ptr<Element> updateChild(
    shared_ptr_ctor<Element> child,
    shared_ptr_ctor<Widget> newWidget,
    void* newSlot
  );
  void updateSlotForChild(
    shared_ptr_ctor<Element> child,
    void* slot
  );
  void updateSlot(void* slot);

  std::shared_ptr<Element> inflateWidget(
    std::shared_ptr<Widget> widget,
    void* slot
  );

  /* Transition from the "inactive" to the "active" lifecycle state.
  
  The framework calls this method when a previously deactivated element has
  been reincorporated into the tree. The framework does not call this method
  the first time an element becomes active (i.e., from the "initial"
  lifecycle state). Instead, the framework calls [mount] in that situation.
  
  See the lifecycle documentation for [Element] for additional information.
  
  Implementations of this method should start with a call to the inherited
  method.
  */
  void activate();
private:
  void _activateWithParent(shared_ptr_ctor<Element> parent, void* slot);
  void _activateRecusively();
public:


  void deactivateChild(shared_ptr_ctor<Element> child);
  /*
Transition from the "active" to the "inactive" lifecycle state.

The framework calls this method when a previously active element is moved to the list of inactive elements. While in the inactive state, the element will not appear on screen. The element can remain in the inactive state only until the end of the current animation frame. At the end of the animation frame, if the element has not be reactivated, the framework will unmount the element.

In case of an uncaught exception when rebuild a widget subtree, the framework also calls this method on the failing subtree to make sure the widget tree is in a relatively consistent state. The deactivation of such subtrees are performed only on a best-effort basis, and the errors thrown during deactivation will not be rethrown.

This is indirectly called by deactivateChild.

See the lifecycle documentation for Element for additional information.

Implementations of this method should end with a call to the inherited method.
  */
  void deactivate() {
    // TODO: InheritedElement when it exists
    m_lifecycleState = _ElementLifecycle::inactive;
  }

  inline bool isDirty() {return m_dirty;}
  virtual std::shared_ptr<BuildScope>& getBuildScope();
  

  /// The default implementation of this function calls attachRenderObject recursively on each child, because, of course, the one in getRenderObject is not its own, nor do it guarantee the parent is a RenderObjectElement. 
  virtual void attachRenderObject(); 
  virtual void detachRenderObject(); 


  /// given that there's only one child
  virtual std::shared_ptr<Element> getAttachingRenderObjectChild();

  /// Calls the argument for each child. Must be overridden by subclasses that
  /// support having children.
  ///
  /// There is no guaranteed order in which the children will be visited, though
  /// it should be consistent over time.
  ///
  /// Calling this during build is dangerous: the child list might still be
  /// being updated at that point, so the children might not be constructed yet,
  /// or might be old children that are going to be replaced. This method should
  /// only be called if it is provable that the children are available. 
  virtual void visitChildren(const std::function<void(shared_ptr_ctor<Element>)>& visitor) {}
};


class RenderObjectElement : public Element {
private:
  cocos2d::CCNode* m_renderObject;
  std::shared_ptr<RenderObjectElement> findAncestorRenderObjectElement();
  std::shared_ptr<RenderObjectElement> m_ancestorRenderObjectElement;

protected:
  /// Creates a render object.
  ///
  /// This is called the first time attachRenderObject is called (unless this function returns nullptr because it just checks for m_renderObject nullability lma)
  ///
  /// # Overriding attachRenderObject
  ///
  /// you dont, override the protected postAttachRenderObject instead. it's the same thing but I did this to ensure the renderOnject is eventually mounted. Do node setup there idk
  virtual cocos2d::CCNode* createRenderObject() = 0;
  virtual void postAttachRenderObject() {}
  virtual void preDetachRenderObject() {}
public:
  void attachRenderObject() final override;
  void detachRenderObject() final override;

  virtual void insertRenderObjectChild(cocos2d::CCNode* child) = 0; 
  virtual void removeRenderObjectChild(cocos2d::CCNode* child) = 0;
};
