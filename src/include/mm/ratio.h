#ifndef MM_RATIO_H
#define MM_RATIO_H

#include <mm/planemap.h>

namespace mm {

  class ratio {
  public:
    double operator()(const planemap<bool>& map);
  };


}


#endif // MM_RATIO_H
