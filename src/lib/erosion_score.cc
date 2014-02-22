#include <mm/erosion_score.h>

#include <cmath>

#include <mm/slope.h>

namespace mm {

  double erosion_score::operator()(const heightmap& src) {
    typedef typename heightmap::size_type size_type;

    heightmap map = slope()(src);

    double total = 0.0;
    double total_sqr = 0.0;
    size_type n = 0;

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        double value = map(x, y);
        total += value;
        total_sqr += value * value;
        n++;
      }
    }

    double avg = total / n;
    double avg_sqr = total_sqr / n;
    double std_dev = std::sqrt(avg_sqr - avg * avg);
    return std_dev / avg;
  }

}
