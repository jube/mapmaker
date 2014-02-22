#include <mm/logical_combine.h>

namespace mm {

  planemap<bool> logical_combine::operator()(const planemap<bool>& lhs, const planemap<bool>& rhs, std::function<bool(bool, bool)> func) {
    typedef typename planemap<bool>::size_type size_type;

    if (lhs.width() != rhs.width()) {
      // TODO
    }

    if (lhs.height() != rhs.height()) {
      // TODO
    }

    planemap<bool> map(lhs.width(), lhs.height());

    for (size_type x = 0; x < map.width(); ++x) {
      for (size_type y = 0; y < map.height(); ++y) {
        map(x, y) = func(lhs(x, y), rhs(x, y));
      }
    }

    return std::move(map);
  }

}
