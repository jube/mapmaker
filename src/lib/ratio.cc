#include <mm/ratio.h>

namespace mm {

  double ratio::operator()(const planemap<bool>& src) {
    typedef typename planemap<bool>::size_type size_type;

    size_type n = 0;

    for (size_type x = 0; x < src.width(); ++x) {
      for (size_type y = 0; y < src.height(); ++y) {
        if (src(x, y)) {
          n++;
        }
      }
    }

    return static_cast<double>(n) / (static_cast<double>(src.width()) * static_cast<double>(src.height()));
  }


}
