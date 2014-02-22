#ifndef MM_SEA_LEVEL_H
#define MM_SEA_LEVEL_H

#include <mm/heightmap.h>

namespace mm {

  class sea_level {
  public:
    sea_level(double level)
    : m_level(level)
    {
    }

    heightmap operator()(const heightmap& src);

  private:
    double m_level;
  };


}


#endif // MM_SEA_LEVEL_H
