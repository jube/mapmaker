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
#ifndef MM_COLORMAP_H
#define MM_COLORMAP_H

#include <iosfwd>

#include <mm/planemap.h>
#include <mm/color.h>

namespace mm {

  class colormap : public planemap<color> {
  public:
    typedef typename planemap<color>::value_type value_type;
    typedef typename planemap<color>::allocator_type allocator_type;
    typedef typename planemap<color>::size_type size_type;
    typedef typename planemap<color>::difference_type difference_type;
    typedef typename planemap<color>::reference reference;
    typedef typename planemap<color>::const_reference const_reference;
    typedef typename planemap<color>::pointer pointer;
    typedef typename planemap<color>::const_pointer const_pointer;

    colormap()
    {
    }

    colormap(size_type w, size_type h)
    : planemap<color>(w, h)
    {
    }

    colormap(size_type w, size_type h, const color& value)
    : planemap<color>(w, h, value)
    {
    }

    colormap(const colormap& other)
    : planemap<color>(other)
    {
    }

    template<typename T>
    colormap(size_only_t, const planemap<T>& other)
    : colormap(other.width(), other.height())
    {
    }

    colormap(colormap&& other)
    : planemap<color>(other)
    {
    }

    colormap& operator=(const colormap& other) {
      if (this == &other) {
        return *this;
      }

      planemap<color>::operator=(other);
      return *this;
    }

    colormap& operator=(colormap&& other) {
      if (this == &other) {
        return *this;
      }

      planemap<color>::operator=(other);
      return *this;
    }

    // specialized methods

    void output_to_ppm(std::ostream& file) const;

  };

}

#endif // MM_COLORMAP_H
