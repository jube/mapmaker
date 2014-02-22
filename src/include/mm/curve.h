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
#ifndef MM_CURVE_H
#define MM_CURVE_H

#include <cmath>

namespace mm {

  template<typename T>
  constexpr
  T lerp(T a, T b, double t) {
    return a * (1 - t) + b * t;
  }

  // x
  template<typename T>
  constexpr
  T curve_linear(T t) {
    return t;
  }

  // -2 * x ** 3 + 3 * x ** 2
  template<typename T>
  constexpr
  T curve_cubic(T t) {
    return -2 * t * t * t + 3 * t * t;
  }

  // 6 * x ** 5 - 15 * x ** 4 + 10 * x ** 3
  template<typename T>
  constexpr
  T curve_quintic(T t) {
    return 6 * t * t * t * t * t - 15 * t * t * t * t + 10 * t * t * t;
  }

  // (1 - cos(pi * x)) * 0.5
  template<typename T>
  constexpr
  T curve_cosine(T t) {
    return (1 - std::cos(M_PI * t)) * 0.5;
  }

}


#endif // MM_CURVE_H
