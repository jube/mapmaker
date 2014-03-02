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
#include <mm/hills.h>

namespace mm {

  heightmap hills::operator()(random_engine& engine, size_type width, size_type height) const {
    heightmap map(width, height, 0.0);

    auto size = std::max(width, height);

    std::uniform_real_distribution<double> dist_radius(m_radius_min * size, m_radius_max * size);

    for (size_type k = 0; k < m_count; ++k) {
      double radius = dist_radius(engine);
      double radius2 = radius * radius;

      std::uniform_real_distribution<double> dist_x(-radius / 2, width + radius / 2);
      double x = dist_x(engine);
      double xmin = x - radius - 1;
      double xmax = x + radius + 1;

      size_type imin = (xmin < 0) ? 0 : static_cast<size_type>(xmin);
      size_type imax = (xmax >= width) ? width : static_cast<size_type>(xmax);

      std::uniform_real_distribution<double> dist_y(-radius / 2, height + radius / 2);
      double y = dist_y(engine);
      double ymin = y - radius - 1;
      double ymax = y + radius + 1;

      size_type jmin = (ymin < 0) ? 0 : static_cast<size_type>(ymin);
      size_type jmax = (ymax >= height) ? height : static_cast<size_type>(ymax);

      for (size_type i = imin; i < imax; ++i) {
        for (size_type j = jmin; j < jmax; ++j) {
          double distance2 = (x - i) * (x - i) + (y - j) * (y - j);
          double value2 = radius2 - distance2;

          if (value2 > 0) {
            map.at(i, j) += value2;
          }
        }
      }
    }

    return std::move(map);
  }

}
