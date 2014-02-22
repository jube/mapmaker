#ifndef MM_ISLANDIZE_H
#define MM_ISLANDIZE_H

#include <mm/heightmap.h>

namespace mm {

  class islandize {
  public:
    typedef std::size_t size_type;

    islandize(double border)
    : m_border(border)
    {
    }

    heightmap operator()(const heightmap& map) const;

  private:
    double m_border;
  };


}


#endif // MM_ISLANDIZE_H
