#include <mm/heightmap.h>

namespace mm {

  heightmap heightmap::submap(size_type x, size_type y, size_type w, size_type h) const {
    if (x + w > this->width()) {
      w = this->width() - x;
    }

    if (y + h > this->height()) {
      h = this->height() - y;
    }

    heightmap sub(w, h);

    for (size_type i = 0; i < w; ++i) {
      for (size_type j = 0; j < h; ++j) {
        sub(i, j) = this->get(x + i, y + j);
      }
    }

    return std::move(sub);
  }

}
