#ifndef MM_NORMALIZE_H
#define MM_NORMALIZE_H

#include <mm/heightmap.h>

namespace mm {

  class normalize {
  public:

    heightmap operator()(const heightmap& src) const;

  };

}

#endif // MM_NORMALIZE_H
