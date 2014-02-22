#include <mm/thermal_erosion.h>

namespace mm {

  heightmap thermal_erosion::operator()(const heightmap& src) const {
    double d[3][3];

    heightmap map(src);
    heightmap material(src.width(), src.height());

    for (size_type k = 0; k < m_iterations; ++k) {
      // initialize material map
      for (size_type x = 1; x < material.width() - 1; ++x) {
        for (size_type y = 1; y < material.height() - 1; ++y) {
          material(x, y) = 0.0;
        }
      }

      // compute material map
      for (size_type x = 1; x < src.width() - 1; ++x) {
        for (size_type y = 1; y < src.height() - 1; ++y) {
          double d_total = 0.0;
          double d_max = 0.0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = map(x, y) - map(x+i, y+j);
              d[1+i][1+j] = diff;

              if (diff > m_talus) {
                d_total += diff;

                if (diff > d_max) {
                  d_max = diff;
                }
              }
            }
          }

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1+i][1+j];

              if (diff > m_talus) {
                material(x+i, y+j) += m_fraction * (d_max - m_talus) * (diff / d_total);
              }
            }
          }
        }
      }

      // add material map to the map
      for (size_type x = 1; x <= map.width() - 2; ++x) {
        for (size_type y = 1; y <= map.height() - 2; ++y) {
          map(x, y) += material(x, y);
        }
      }
    }

    return std::move(map);
  }

}
