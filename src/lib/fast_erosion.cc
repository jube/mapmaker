#include <mm/fast_erosion.h>

namespace mm {

  heightmap fast_erosion::operator()(const heightmap& src) const {
    heightmap map(src);
    heightmap material(src.width(), src.height());

    for (size_type k = 0; k < m_iterations; ++k) {
      // initialize material map
      for (size_type x = 1; x <= material.width() - 2; ++x) {
        for (size_type y = 1; y <= material.height() - 2; ++y) {
          material(x, y) = 0.0;
        }
      }

      // compute material map
      for (size_type x = 1; x <= src.width() - 2; ++x) {
        for (size_type y = 1; y <= src.height() - 2; ++y) {
          double d_max = 0.0;
          size_type x_max = x;
          size_type y_max = y;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = map(x, y) - map(x+i, y+j);

              if (diff > d_max) {
                d_max = diff;
                x_max = x + i;
                y_max = y + j;
              }
            }
          }

          if (0 < d_max && d_max <= m_talus) {
            material(x, y) -= m_fraction * d_max;
            material(x_max, y_max) += m_fraction * d_max;
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
