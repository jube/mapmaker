#include <mm/normalize.h>

namespace mm {

  heightmap normalize::operator()(const heightmap& src) const {
    typedef typename heightmap::size_type size_type;

    heightmap map(src.width(), src.height());

    auto max = src(0, 0);
    auto min = src(0, 0);

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        double value = src(x, y);

        if (value < min) {
          min = value;
        }

        if (value > max) {
          max = value;
        }
      }
    }

//     std::cerr << "min: " << min << '\n';
//     std::cerr << "max: " << max << '\n';

    for (size_type x = 0; x < map.width(); ++x) {
      for (size_type y = 0; y < map.height(); ++y) {
        map(x, y) = (src(x, y) - min) / (max - min);
      }
    }

    return std::move(map);
  }

}
