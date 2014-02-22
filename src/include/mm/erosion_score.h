#ifndef MM_EROSION_SCORE_H
#define MM_EROSION_SCORE_H

#include <mm/heightmap.h>

namespace mm {

  class erosion_score {
  public:

    double operator()(const heightmap& src);

  };

}

#endif // MM_EROSION_SCORE_H
