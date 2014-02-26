/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef MM_HEIGHTMAP_H
#define MM_HEIGHTMAP_H

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

    template<typename T>
    heightmap(size_only_t, const planemap<T>& other)
    : heightmap(other.width(), other.height())
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
