#ifndef MM_REACHABILITY_H
#define MM_REACHABILITY_H

#include <mm/planemap.h>

namespace mm {

  class reachability {
  public:
    typedef std::size_t size_type;

    reachability(size_type size)
    : m_size(size)
    {
    }

    planemap<bool> operator()(const planemap<bool>& src) const;

  private:
    size_type m_size;
  };

}

#endif // MM_REACHABILITY_H
