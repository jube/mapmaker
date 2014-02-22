#include <mm/sea_level.h>

namespace mm {

  heightmap sea_level::operator()(const heightmap& src) {
    typedef typename heightmap::size_type size_type;

    heightmap map(src.width(), src.height());

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        double value = src(x, y);

        if (value < m_level) {
          map(x, y) = value / m_level * 0.5;
        } else {
          map(x, y) = (value - m_level) / (1.0 - m_level) * 0.5 + 0.5;
        }

      }
    }

    return std::move(map);
  }


}
