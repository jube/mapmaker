#ifndef MM_SLOPE_H
#define MM_SLOPE_H

#include <mm/heightmap.h>

namespace mm {

  class slope {
  public:

    heightmap operator()(const heightmap& src);

  };

}

#endif // MM_SLOPE_H
