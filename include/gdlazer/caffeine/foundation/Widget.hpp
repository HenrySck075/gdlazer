#pragma once

#include "Key.hpp"
#include "utils/readonly.hpp"
#include "utils/shared_ptr_2.hpp"
#include <memory>
class Element;
class BuildContext;

class Widget {
public:
  readonly<std::shared_ptr<Key>, Element> m_key;
  virtual std::shared_ptr<Element> createElement() {return nullptr;};
  static bool canUpdate(shared_ptr_ctor<Widget> newWidget,
                        shared_ptr_ctor<Widget> oldWidget);
};





class StatelessWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
  virtual std::shared_ptr<Widget> build(std::shared_ptr<BuildContext> context);
};
