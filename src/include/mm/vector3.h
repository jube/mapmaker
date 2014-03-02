/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef MM_VECTOR3_H
#define MM_VECTOR3_H

#include <cmath>

namespace mm {

  struct vector3 {
    double x;
    double y;
    double z;
  };

  inline vector3 operator+(const vector3& lhs, const vector3& rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
  }

  inline vector3 operator-(const vector3& lhs, const vector3& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
  }

  inline
  double dot(const vector3& lhs, const vector3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  inline
  vector3 cross(const vector3& lhs, const vector3& rhs) {
    return {
      lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x
    };
  }

  inline
  double norm(const vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  }

  inline
  vector3 unit(const vector3& v) {
    double n = norm(v);
    return {v.x / n, v.y / n, v.z / n};
  }

}

#endif // MM_VECTOR3_H
