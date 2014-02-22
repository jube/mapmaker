#ifndef MM_LOGICAL_COMBINE_H
#define MM_LOGICAL_COMBINE_H

#include <functional>

#include <mm/planemap.h>

namespace mm {

  class logical_combine {
  public:
    planemap<bool> operator()(const planemap<bool>& lhs, const planemap<bool>& rhs, std::function<bool(bool, bool)> func);
  };


}

#endif // MM_LOGICAL_COMBINE_H
