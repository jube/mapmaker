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
#include <mm/islandize.h>

#include <cmath>

namespace mm {

  heightmap islandize::operator()(const heightmap& src) const {
    heightmap map(src);

    for (size_type x = 0; x < map.width(); ++x) {
      for (size_type y = 0; y < map.height(); ++y) {
        double coeffx = 1.0;

        if (x < m_border) {
          coeffx = static_cast<double>(x)/static_cast<double>(m_border);
        } else if (x + m_border > map.width()) {
          coeffx = static_cast<double>(map.width() - x + 1)/static_cast<double>(m_border);
        }

        double coeffy = 1.0;

        if (y < m_border) {
          coeffy = static_cast<double>(y)/static_cast<double>(m_border);
        } else if (y + m_border > map.height()) {
          coeffy = static_cast<double>(map.height() - y + 1)/static_cast<double>(m_border);
        }

        double coeff = coeffx * coeffy;

        if (coeff < 1.0) {
          map(x, y) = map(x, y) * std::sin(std::sqrt(coeff) * M_PI / 2);
        }
      }
    }

    return map;
  }

}
