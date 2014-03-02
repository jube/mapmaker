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
#include <mm/fractal.h>

namespace mm {

  heightmap fractal::operator()(random_engine& engine, size_type width, size_type height) const {
    heightmap map(width, height);

    for (size_type x = 0; x < width; ++x) {
      for (size_type y = 0; y < height; ++y) {
        double value = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;

        const double xf = static_cast<double>(x) / static_cast<double>(width) * m_scale;
        const double yf = static_cast<double>(y) / static_cast<double>(height) * m_scale;

        for (size_type k = 0; k < m_octaves; ++k) {
          value += m_noise(xf * frequency, yf * frequency) * amplitude;
          frequency *= m_lacunarity;
          amplitude *= m_persistence;
        }

        map(x, y) = value;
      }
    }

    return std::move(map);
  }

}
