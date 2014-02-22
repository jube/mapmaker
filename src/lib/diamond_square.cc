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
#include <mm/diamond_square.h>

namespace mm {

  typedef typename diamond_square::size_type size_type;

  static void diamond(random_engine& engine, heightmap& map, size_type x, size_type y, size_type d) {
    double value = (map(x - d, y - d) + map(x - d, y + d) + map(x + d, y - d) + map(x + d, y + d)) / 4;

    std::uniform_real_distribution<double> dist(-static_cast<double>(d), static_cast<double>(d));
    double noise = dist(engine);

    map(x, y) = value + noise;
  }

  static void square(random_engine& engine, heightmap& map, size_type x, size_type y, size_type d) {
    double value = 0.0;
    size_type n = 0;

    if (x >= d) {
      value += map(x - d, y);
      n++;
    }

    if (x + d < map.width()) {
      value += map(x + d, y);
      n++;
    }

    if (y >= d) {
      value += map(x, y - d);
      n++;
    }

    if (y + d < map.height()) {
      value += map(x, y + d);
      n++;
    }

    value = value / n;

    std::uniform_real_distribution<double> dist(-static_cast<double>(d), static_cast<double>(d));
    double noise = dist(engine);

    map(x, y) = value + noise;
  }

  heightmap diamond_square::operator()(random_engine& engine, size_type width, size_type height) const {
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

      for (size_type x = d_2; x < map.width(); x += d) {
        for (size_type y = d_2; y < map.height(); y += d) {
          diamond(engine, map, x, y, d_2);
        }
      }

      for (size_type x = d_2; x < map.width(); x += d) {
        for (size_type y = 0; y < map.height(); y += d) {
          square(engine, map, x, y, d_2);
        }
      }

      for (size_type x = 0; x < map.width(); x += d) {
        for (size_type y = d_2; y < map.height(); y += d) {
          square(engine, map, x, y, d_2);
        }
      }

      d = d_2;
    }

    size_type offset_x = (size - width) / 2;
    size_type offset_y = (size - height) / 2;

    return std::move(map.submap(offset_x, offset_y, width, height));
  }

}
