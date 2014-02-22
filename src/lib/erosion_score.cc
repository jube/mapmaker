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
#include <mm/erosion_score.h>

#include <cmath>

#include <mm/slope.h>

namespace mm {

  double erosion_score::operator()(const heightmap& src) {
    typedef typename heightmap::size_type size_type;

    heightmap map = slope()(src);

    double total = 0.0;
    double total_sqr = 0.0;
    size_type n = 0;

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        double value = map(x, y);
        total += value;
        total_sqr += value * value;
        n++;
      }
    }

    double avg = total / n;
    double avg_sqr = total_sqr / n;
    double std_dev = std::sqrt(avg_sqr - avg * avg);
    return std_dev / avg;
  }

}
