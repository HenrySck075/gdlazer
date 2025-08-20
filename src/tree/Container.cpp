#include "Container.hpp"
#include "Element.hpp"

ElementPtr Container::createElement() {
  return new Element(this);
};
