#pragma once
#include <initializer_list>
#undef rad1
#undef max
#undef min
#define HOMOG2D_ENABLE_VRTP
#include "homog2d.hpp"

/// not really considered part of gdl framework

template<typename FPT>
struct h2dShapeContainer {
  // we're not even changing the size here
  std::vector<h2d::CommonType_<FPT>> shapes;
  h2dShapeContainer() {};
  h2dShapeContainer(std::initializer_list<h2d::CommonType_<FPT>> shapes)
    : shapes(shapes) {};
	template<typename T>
	bool isSomethingInside(const h2d::Point2d_<T>& something) const
	{
    // TODO: this always return false because it's using the fallback method from Common
    //
#define shapeTypeOf(type) std::holds_alternative<type>(shape) ? something.isInside(std::get<type>(shape))
    for (h2d::CommonType_<FPT> shape : shapes) {
      if (
        shapeTypeOf(h2d::Line2d_<FPT>) :
        shapeTypeOf(h2d::FRect_<FPT>) :
        shapeTypeOf(h2d::Circle_<FPT>) :
        shapeTypeOf(h2d::CPolyline_<FPT>) :
        false
      ) {
        //
        return true;
      }
      //
    }
    return false;
	}
};
