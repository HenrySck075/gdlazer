#include "ElementTree.hpp"
#include "foundation/Element.hpp"
#include "foundation/BuildScope.hpp"

void ElementTree::scheduleBuildFor(shared_ptr_ctor<Element> element) {
  element->getBuildScope()->scheduleBuildFor(element);
}
