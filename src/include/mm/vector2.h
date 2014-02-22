#ifndef MM_VECTOR2_H
#define MM_VECTOR2_H

#include <iostream>

#include <cmath>

namespace mm {

  struct vector2 {
    double x;
    double y;
  };

  inline
  double norm(const vector2& vec) {
    return std::hypot(vec.x, vec.y);
  }

  inline
  double dot(const vector2& lhs, const vector2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

}

#endif // MM_VECTOR2_H
