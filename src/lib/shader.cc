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
#include <mm/shader.h>

#include <iostream>
#include <fstream>

#include <cassert>

#include <mm/normalize.h>
#include <mm/curve.h>

namespace mm {

  static color lerp(const color& lhs, const color& rhs, double t) {
    return {
      lerp(lhs.red_channel(), rhs.red_channel(), t),
      lerp(lhs.green_channel(), rhs.green_channel(), t),
      lerp(lhs.blue_channel(), rhs.blue_channel(), t),
      lerp(lhs.alpha_channel(), rhs.alpha_channel(), t)
    };
  }

  static const vector3 light = {-1, -1, 0};

  colormap shader::operator()(const colormap& src, const heightmap& map) const {
    assert(src.width() == map.width());
    assert(src.height() == map.height());

    heightmap factor(size_only, map);

    for (heightmap::size_type x = 0; x < map.width(); ++x) {
      for (heightmap::size_type y = 0; y < map.height(); ++y) {
        double xx = x;
        double yy = y;

        // compute the normal vector
        double nx = 0;
        double ny = 0;
        double nz = 0;
        unsigned count = 0;

        vector3 p{xx, yy, map(x, y)};

        if (x > 0 && y > 0) {
          vector3 pw{xx - 1, yy    , map(x - 1, y    )};
          vector3 pn{xx    , yy - 1, map(x    , y - 1)};

          vector3 v3 = cross(p - pw, p - pn);
          assert(v3.z > 0);

          nx += v3.x;
          ny += v3.y;
          nz += v3.z;
          count += 1;
        }

        if (x > 0 && y < src.height() - 1) {
          vector3 pw{xx - 1, yy    , map(x - 1, y    )};
          vector3 ps{xx    , yy + 1, map(x    , y + 1)};

          vector3 v3 = cross(p - ps, p - pw);
          assert(v3.z > 0);

          nx += v3.x;
          ny += v3.y;
          nz += v3.z;
          count += 1;
        }

        if (x < src.width() - 1 && y > 0) {
          vector3 pe{xx + 1, yy    , map(x + 1, y    )};
          vector3 pn{xx    , yy - 1, map(x    , y - 1)};

          vector3 v3 = cross(p - pn, p - pe);
          assert(v3.z > 0);

          nx += v3.x;
          ny += v3.y;
          nz += v3.z;
          count += 1;
        }

        if (x < src.width() - 1 && y < src.height() - 1) {
          vector3 pe{xx + 1, yy    , map(x + 1, y    )};
          vector3 ps{xx    , yy + 1, map(x    , y + 1)};

          vector3 v3 = cross(p - pe, p - ps);
          assert(v3.z > 0);

          nx += v3.x;
          ny += v3.y;
          nz += v3.z;
          count += 1;
        }

        vector3 normal = unit({nx / count, ny / count, nz / count});
        double d = dot(light, normal);
        d = 0.5 + 35 * d;

        if (d > 1) {
          d = 1;
        }

        if (d < 0) {
          d = 0;
        }

        factor(x, y) = d;
      }
    }

    colormap result(size_only, src);

    for (colormap::size_type x = 0; x < src.width(); ++x) {
      for (colormap::size_type y = 0; y < src.height(); ++y) {
        if (map(x, y) < m_sea_level) {
          result(x, y) = src(x, y);
          continue;
        }

        double d = factor(x, y);

        auto lo = lerp(src(x, y), {0x33, 0x11, 0x33}, 0.7);
        auto hi = lerp(src(x, y), {0xFF, 0xFF, 0xCC}, 0.3);

        if (d < 0.5) {
          result(x, y) = lerp(lo, src(x, y), 2 * d);
        } else {
          result(x, y) = lerp(src(x, y), hi, 2 * d - 1);
        }

      }
    }

    return std::move(result);
  }

}
