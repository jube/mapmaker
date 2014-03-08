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
#include <mm/hydraulic_erosion.h>

namespace mm {

  heightmap hydraulic_erosion::operator()(const heightmap& src) const {
    heightmap water_map(size_only, src);
    heightmap water_diff(size_only, src);

    heightmap material_map(size_only, src);
    heightmap material_diff(size_only, src);

    heightmap map(src);

    double d[3][3];

    for (size_type k = 0; k < m_iterations; ++k) {

      // 1. appearance of new water
      for (size_type x = 0; x < water_map.width(); ++x) {
        for (size_type y = 0; y < water_map.height(); ++y) {
          water_map(x, y) += m_rain_amount;
        }
      }

      // 2. water erosion of the terrain
      for (size_type x = 0; x < water_map.width(); ++x) {
        for (size_type y = 0; y < water_map.height(); ++y) {
          double material = m_solubility * water_map(x, y);
          map(x, y) -= material;
          material_map(x, y) += material;
        }
      }

      // 3. transportation of water
      for (size_type x = 1; x < water_diff.width() - 1; ++x) {
        for (size_type y = 1; y < water_diff.height() - 1; ++y) {
          water_diff(x, y) = 0.0;
        }
      }

      for (size_type x = 1; x < material_diff.width() - 1; ++x) {
        for (size_type y = 1; y < material_diff.height() - 1; ++y) {
          material_diff(x, y) = 0.0;
        }
      }

      for (size_type x = 1; x < map.width() - 1; ++x) {
        for (size_type y = 1; y < map.height() - 1; ++y) {
          double d_total = 0.0;
          double a_total = 0.0;
          double alt = map(x, y) + water_map(x, y);
          size_type n = 0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double alt_local = map(x+i, y+j) + water_map(x+i, y+j);
              double diff = alt - alt_local;
              d[1+i][1+j] = diff;

              if (diff > 0.0) {
                d_total += diff;
                a_total += alt_local;
                n++;
              }
            }
          }

          if (n == 0) {
            continue;
          }

          double a_avg = a_total / n;
          double da = std::min(water_map(x, y), alt - a_avg);

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1+i][1+j];

              if (diff > 0.0) {
                double dw = da * (diff / d_total);
                water_diff(x+i, y+j) += dw;
                water_diff(x, y) -= dw;

                double dm = material_map(x, y) * (dw / water_map(x, y));
                material_diff(x+i, y+j) += dm;
                material_diff(x, y) -= dm;
              }
            }
          }
        }
      }

      for (size_type x = 1; x < water_map.width() - 1; ++x) {
        for (size_type y = 1; y < water_map.height() - 1; ++y) {
          water_map(x, y) += water_diff(x, y);
        }
      }

      for (size_type x = 1; x < material_map.width() - 1; ++x) {
        for (size_type y = 1; y < material_map.height() - 1; ++y) {
          material_map(x, y) += material_diff(x, y);
        }
      }

      // 4. evaporation of water
      for (size_type x = 0; x < water_map.width(); ++x) {
        for (size_type y = 0; y < water_map.height(); ++y) {
          double water = water_map(x, y) * (1 - m_evaporation);

          water_map(x, y) = water;

          double m_max = m_capacity * water;
          double dm = std::max(double(0), material_map(x, y) - m_max);
          material_map(x, y) -= dm;
          map(x, y) += dm;
        }
      }

    }

    return map;
  }

}
