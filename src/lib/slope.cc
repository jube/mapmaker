#include <mm/slope.h>

namespace mm {

  heightmap slope::operator()(const heightmap& src) {
    typedef typename heightmap::size_type size_type;

    heightmap map(src.width(), src.height());

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        double value = 0.0;

        for (int i = -1; i <= 1; ++i) {
          if (x == 0 && i == -1) {
            continue;
          }

          if (x == src.width() - 1 && i == 1) {
            continue;
          }

          for (int j = -1; j <= 1; ++j) {
            if (y == 0 && j == -1) {
              continue;
            }

            if (y == src.height() - 1 && j == 1) {
              continue;
            }

            double diff = std::abs(src(x, y) - src(x+i, y+j));

            if (diff > value) {
              value = diff;
            }
          }
        }

        map(x, y) = value;
      }
    }

    return std::move(map);
  }

}
