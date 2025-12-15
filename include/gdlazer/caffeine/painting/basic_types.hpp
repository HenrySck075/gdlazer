#pragma once
#include "../dartui/basic_types.hpp"

namespace caffeine {
  enum class Axis : char {
    horizontal = 0, vertical = 1
  };
  inline Axis flipAxis(Axis axis) {
    return static_cast<Axis>((static_cast<char>(axis)+1)%2);
  }


  enum class VerticalDirection : char {up, down};
  enum class AxisDirection : char {up,down,left,right};
  inline Axis axisDirectionToAxis(AxisDirection axisDir) {
    switch (axisDir) {
      case AxisDirection::up: 
      case AxisDirection::down:
        return Axis::vertical;
      case AxisDirection::left:
      case AxisDirection::right:
        return Axis::horizontal;
    }
  }
  inline AxisDirection flipAxisDirection(AxisDirection axisDir) {
    switch (axisDir) {
      case AxisDirection::up:
        return AxisDirection::down;
      case AxisDirection::down:
        return AxisDirection::up;
      case AxisDirection::left:
        return AxisDirection::right;
      case AxisDirection::right:
        return AxisDirection::left;
    }
  }
  inline bool axisDirectionIsReversed(AxisDirection axisDir) {
    switch (axisDir) {
      case AxisDirection::up:
      case AxisDirection::left:
        return true;
      case AxisDirection::down:
      case AxisDirection::right:
        return false;
    }
  }
}
