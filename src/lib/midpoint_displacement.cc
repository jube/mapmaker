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
#include <mm/midpoint_displacement.h>

namespace mm {

  heightmap midpoint_displacement::operator()(random_engine& engine, size_type width, size_type height) const {
    size_type size = 1;

    while (size + 1 < height || size + 1 < width) {
      size = size * 2;
    }

    size_type d = size;
    size = size + 1;
    heightmap map(size, size);

    map(0, 0) = m_ne;
    map(0, d) = m_nw;
    map(d, 0) = m_se;
    map(d, d) = m_sw;

    while (d >= 2) {
      size_type d_2 = d / 2;

      std::uniform_real_distribution<double> dist(-static_cast<double>(d), static_cast<double>(d));

      for (size_type x = d_2; x < map.width(); x += d) {
        for (size_type y = d_2; y < map.height(); y += d) {
          double ne = map(x - d_2, y - d_2);
          double nw = map(x - d_2, y + d_2);
          double se = map(x + d_2, y - d_2);
          double sw = map(x + d_2, y + d_2);

          // center
          double center = (ne + nw + se + sw) / 4;
          map(x, y) = center + dist(engine);

          // north
          double north = (ne + nw) / 2;
          map(x - d_2, y) = north + dist(engine);

          // south
          double south = (se + sw) / 2;
          map(x + d_2, y) = south + dist(engine);

          // east
          double east = (ne + se) / 2;
          map(x, y - d_2) = east + dist(engine);

          // west
          double west = (nw + sw) / 2;
          map(x, y + d_2) = west + dist(engine);
        }
      }

      d = d_2;
    }

    size_type offset_x = (size - width) / 2;
    size_type offset_y = (size - height) / 2;

    return std::move(map.submap(offset_x, offset_y, width, height));
  }

}
