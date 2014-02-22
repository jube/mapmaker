
#ifndef MM_DIAMOND_SQUARE_H
#define MM_DIAMOND_SQUARE_H

#include <mm/random.h>
#include <mm/heightmap.h>

namespace mm {

  class diamond_square {
  public:
    typedef typename position::size_type size_type;

    diamond_square(double val = 0.0)
    : m_ne(val), m_nw(val), m_sw(val), m_se(val)
    {
    }

    diamond_square(double ne, double nw, double sw, double se)
    : m_ne(ne), m_nw(nw), m_sw(sw), m_se(se)
    {
    }

    heightmap operator()(random_engine& r, size_type width, size_type height) const;

  private:
    double m_ne;
    double m_nw;
    double m_sw;
    double m_se;
  };


}


#endif // MM_DIAMOND_SQUARE_H
