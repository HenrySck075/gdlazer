#pragma once

#include "WidgetsBinding.hpp"
#include "SchedulerBinding.hpp"

namespace caffeine {

/// Combines WidgetsBinding and SchedulerBinding into a single object.
/// WidgetsContainer owns this instead of inheriting from multiple binding classes.
class WidgetsFlutterBinding : public WidgetsBinding, public SchedulerBinding {
public:
  WidgetsFlutterBinding() = default;
  virtual ~WidgetsFlutterBinding() = default;

  // Inherit all methods from both bindings
};

}  // namespace caffeine
