
#ifndef MM_HEIGHTMAP_H
#define MM_HEIGHTMAP_H

#include <cmath>
#include <memory>
#include <stdexcept>

#include <mm/planemap.h>

namespace mm {

  class heightmap : public planemap<double> {
  public:
    typedef typename planemap<double>::value_type value_type;
    typedef typename planemap<double>::allocator_type allocator_type;
    typedef typename planemap<double>::size_type size_type;
    typedef typename planemap<double>::difference_type difference_type;
    typedef typename planemap<double>::reference reference;
    typedef typename planemap<double>::const_reference const_reference;
    typedef typename planemap<double>::pointer pointer;
    typedef typename planemap<double>::const_pointer const_pointer;

    heightmap()
    {
    }

    heightmap(size_type w, size_type h)
    : planemap<double>(w, h)
    {
    }

    heightmap(size_type w, size_type h, double value)
    : planemap<double>(w, h, value)
    {
    }

    heightmap(const heightmap& other)
    : planemap<double>(other)
    {
    }

    heightmap(heightmap&& other)
    : planemap<double>(other)
    {
    }

    heightmap& operator=(const heightmap& other) {
      if (this == &other) {
        return *this;
      }

      planemap<double>::operator=(other);
      return *this;
    }

    heightmap& operator=(heightmap&& other) {
      if (this == &other) {
        return *this;
      }

      planemap<double>::operator=(other);
      return *this;
    }

    // specialized methods

    heightmap submap(size_type x, size_type y, size_type w, size_type h) const;
  };

}

#endif // MM_HEIGHTMAP_H
