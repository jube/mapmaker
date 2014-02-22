#include <mm/cutoff.h>

namespace mm {


  planemap<bool> cutoff::operator()(const heightmap& src) const {
    typedef typename heightmap::size_type size_type;

    planemap<bool> map(src.width(), src.height());

    for (size_type x = 0; x < map.width(); ++x) {
      for (size_type y = 0; y < map.height(); ++y) {
        map(x, y) = (src(x, y) < m_threshold);
      }
    }

    return std::move(map);
  }

}