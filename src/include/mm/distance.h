#ifndef MM_DISTANCE_H
#define MM_DISTANCE_H

#include <cmath>

#include <mm/vector2.h>

namespace mm {

  inline
  double distance_manhattan(const vector2& a, const vector2& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }

  inline
  double distance_euclidean(const vector2& a, const vector2& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
  }

  inline
  double distance_chebyshev(const vector2& a, const vector2& b) {
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
  }

}

#endif // MM_DISTANCE_H
