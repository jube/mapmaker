#include <mm/islandize.h>

#include <mm/curve.h>

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

//         double coeff = std::min(coeffx, coeffy);
//         double coeff = std::hypot(coeffx, coeffy);
        double coeff = coeffx * coeffy;

        if (coeff < 1.0) {
//           map(x, y) = map(x, y) * std::sqrt(coeff);
//           map(x, y) = map(x, y) * curve_cubic(coeff);
          map(x, y) = map(x, y) * std::sin(std::sqrt(coeff) * M_PI / 2);
        }
      }
    }

    return std::move(map);
  }

}
