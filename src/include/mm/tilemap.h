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
#ifndef MM_TILEMAP_H
#define MM_TILEMAP_H

#include <mm/planemap.h>
#include <mm/tile.h>

namespace mm {

  class tilemap : public planemap<tile> {
  public:
    typedef typename planemap<tile>::value_type value_type;
    typedef typename planemap<tile>::allocator_type allocator_type;
    typedef typename planemap<tile>::size_type size_type;
    typedef typename planemap<tile>::difference_type difference_type;
    typedef typename planemap<tile>::reference reference;
    typedef typename planemap<tile>::const_reference const_reference;
    typedef typename planemap<tile>::pointer pointer;
    typedef typename planemap<tile>::const_pointer const_pointer;

    tilemap()
    {
    }

    tilemap(size_type w, size_type h)
    : planemap<tile>(w, h)
    {
    }

    tilemap(size_type w, size_type h, tile value)
    : planemap<tile>(w, h, value)
    {
    }

    tilemap(const tilemap& other)
    : planemap<tile>(other)
    {
    }

    template<typename T>
    tilemap(size_only_t, const planemap<T>& other)
    : tilemap(other.width(), other.height())
    {
    }

    tilemap(tilemap&& other)
    : planemap<tile>(other)
    {
    }

    tilemap& operator=(const tilemap& other) {
      if (this == &other) {
        return *this;
      }

      planemap<tile>::operator=(other);
      return *this;
    }

    tilemap& operator=(tilemap&& other) {
      if (this == &other) {
        return *this;
      }

      planemap<tile>::operator=(other);
      return *this;
    }

    // specialized methods
  };

}

#endif // MM_TILEMAP_H
