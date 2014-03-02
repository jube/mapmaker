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
#ifndef MM_BINARYMAP_H
#define MM_BINARYMAP_H

#include <iosfwd>

#include <mm/planemap.h>

namespace mm {

  class binarymap : public planemap<bool> {
  public:
    typedef typename planemap<bool>::value_type value_type;
    typedef typename planemap<bool>::allocator_type allocator_type;
    typedef typename planemap<bool>::size_type size_type;
    typedef typename planemap<bool>::difference_type difference_type;
    typedef typename planemap<bool>::reference reference;
    typedef typename planemap<bool>::const_reference const_reference;
    typedef typename planemap<bool>::pointer pointer;
    typedef typename planemap<bool>::const_pointer const_pointer;

    binarymap()
    {
    }

    binarymap(size_type w, size_type h)
    : planemap<bool>(w, h)
    {
    }

    binarymap(size_type w, size_type h, bool value)
    : planemap<bool>(w, h, value)
    {
    }

    binarymap(const binarymap& other)
    : planemap<bool>(other)
    {
    }

    template<typename T>
    binarymap(size_only_t, const planemap<T>& other)
    : binarymap(other.width(), other.height())
    {
    }

    binarymap(binarymap&& other)
    : planemap<bool>(other)
    {
    }

    binarymap& operator=(const binarymap& other) {
      if (this == &other) {
        return *this;
      }

      planemap<bool>::operator=(other);
      return *this;
    }

    binarymap& operator=(binarymap&& other) {
      if (this == &other) {
        return *this;
      }

      planemap<bool>::operator=(other);
      return *this;
    }

    // specialized methods

    void output_to_pbm(const std::string& filename) const;
    void output_to_pbm(std::ostream& file) const;

  };

}

#endif // MM_BINARYMAP_H
