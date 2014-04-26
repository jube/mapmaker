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
#include <mm/fast_erosion.h>

namespace mm {

  heightmap fast_erosion::operator()(const heightmap& src) const {
    heightmap map(src);
    heightmap material(size_only, src);

    for (size_type k = 0; k < m_iterations; ++k) {
      // initialize material map
      material.reset(0.0);

      // compute material map
      for (auto x : src.x_range()) {
        for (auto y : src.y_range()) {
          double altitude_difference_max = 0.0;
          position pos_max = { x, y };

          const double altitude_here = map(x, y);

          map.visit8neighbours(x, y, [altitude_here, &altitude_difference_max, &pos_max](position pos, double altitude_there) {
            double altitude_difference = altitude_here - altitude_there;
            if (altitude_difference > altitude_difference_max) {
              altitude_difference_max = altitude_difference;
              pos_max = pos;
            }
          });

          if (0 < altitude_difference_max && altitude_difference_max <= m_talus) {
            material(x, y) -= m_fraction * altitude_difference_max;
            material(pos_max) += m_fraction * altitude_difference_max;
          }

        }
      }

      // add material map to the map
      for (auto fp : map.positions()) {
        map(fp) += material(fp);
      }
    }

    return map;
  }

}
