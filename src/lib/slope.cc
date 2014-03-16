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
#include <mm/slope.h>

#include <cmath>

namespace mm {

  heightmap slope::operator()(const heightmap& src) {
    heightmap map(size_only, src);

    for (heightmap::size_type x = 0; x < src.width(); ++x) {
      for (heightmap::size_type y = 0; y < src.height(); ++y) {
        const double altitude_here = src(x, y);
        double altitude_difference_max = 0.0;

        src.visit8neighbours(x, y, [altitude_here, &altitude_difference_max](position pos, double altitude_there) {
          double altitude_difference = std::abs(altitude_here - altitude_there);
          if (altitude_difference > altitude_difference_max) {
            altitude_difference_max = altitude_difference;
          }
        });

        map(x, y) = altitude_difference_max;
      }
    }

    return map;
  }

}
