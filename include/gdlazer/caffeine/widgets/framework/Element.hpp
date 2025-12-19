#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <Geode/cocos/include/cocos2d.h>
#include <gdlazer/caffeine/foundation/utils/shared_ptr_2.hpp>
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>
#include <gdlazer/caffeine/foundation/Key.hpp>
#include <gdlazer/caffeine/rendering/object/RenderObject.hpp>
#include "BuildContext.hpp"

namespace caffeine {

class Widget;
class Element;
class BuildOwner;
class BuildScope;
using ElementVisitor = std::function<void(RefNauseam<Element>)>;
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
  friend class BuildOwner;
  std::list<RefNauseam<Element>> m_elements;
  bool m_locked = false;

  static void _unmount(RefNauseam<Element> element);
  void _unmountAll();

  void _deactivateRecursively(RefNauseam<Element> element);

public:
  void add(RefNauseam<Element> element);
  void remove(RefNauseam<Element> element);
};

class Element : public BuildContext, public log::StringConvertible {
  friend class _InactiveElements;
protected:
  std::shared_ptr<BuildScope> m_parentBuildScope;
  std::shared_ptr<BuildOwner> m_owner;
  RefNauseam<Element> m_parent; friend class RenderObjectElement;
  RefNauseam<Widget> m_widget;
  /// idk what do they mean by this their usage is confusing
  _ElementLifecycle m_lifecycleState = _ElementLifecycle::initial;
  bool m_dirty = true;
  bool m_inDirtyList = false; friend class BuildScope;
  int m_depth = 0;

  void* m_slot = 0;

  RefNauseam<Element> _retakeInactiveElement(
    GlobalKeyU* key, Widget* widget
  );
public:
  static bool _sort(RefNauseam<Element> a, RefNauseam<Element> b) {
    if (int diff = a->m_depth - b->m_depth) return diff < 0;
    bool bDirty = b->m_dirty;
    if (a->m_dirty != bDirty) return bDirty;
    return true;
  }


  Element(RefNauseam<Widget> widget);
  virtual ~Element() = default;

  /// Get the render object at current (or below) element
  virtual RefNauseam<caffeine::RenderObject> getRenderObject();

  virtual void mount(RefNauseam<Element> parent, void* slot);
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

  virtual void update(Widget *newWidget);
  RefNauseam<Element> updateChild(
    RefNauseam<Element> child,
    Widget* newWidget,
    void* newSlot
  );
  void updateSlotForChild(
    RefNauseam<Element> child,
    void* slot
  );
  void updateSlot(void* slot);

  RefNauseam<Element> inflateWidget(
    Widget* widget,
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
  void _activateWithParent(Element* parent, void* slot);
  void _activateRecusively();
public:


  void deactivateChild(RefNauseam<Element> child);
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
  

  /// Attach this element's render object to the render tree
  /// The default implementation of this function calls attachRenderObject recursively on each child, because, of course, the one in getRenderObject is not its own, nor do it guarantee the parent is a RenderObjectElement. 
  virtual void attachRenderObject(void* newSlot); 
  /// Detach this element's render object from the render tree
  /// The default implementation of this function calls detachRenderObject recursively on each child, because, of course, the one in getRenderObject is not its own, nor do it guarantee the parent is a RenderObjectElement. 
  virtual void detachRenderObject(); 

  /// Get the child element that has an attached render object
  virtual RefNauseam<Element> getAttachingRenderObjectChild();

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
  virtual void visitChildren(std::function<void(RefNauseam<Element>)> visitor) {}
};

}
