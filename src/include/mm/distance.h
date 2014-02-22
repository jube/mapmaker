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
