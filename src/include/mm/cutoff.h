#ifndef MM_CUTOFF_OPERATOR_H
#define MM_CUTOFF_OPERATOR_H

#include <mm/heightmap.h>

namespace mm {

  class cutoff {
  public:
    cutoff(double threshold)
    : m_threshold(threshold)
    {
    }

    planemap<bool> operator()(const heightmap& src) const;

  private:
    double m_threshold;
  };

}

#endif // MM_CUTOFF_OPERATOR_H
