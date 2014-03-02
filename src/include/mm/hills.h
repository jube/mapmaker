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
#ifndef MM_HILS_H
#define MM_HILS_H

#include <mm/heightmap.h>
#include <mm/random.h>

namespace mm {

  class hills {
  public:
    typedef std::size_t size_type;

    hills(size_type count, double radius_min, double radius_max)
    : m_count(count), m_radius_min(radius_min), m_radius_max(radius_max)
    {
      // just in case
      if (m_radius_min > m_radius_max) {
        std::swap(m_radius_min, m_radius_max);
      }
    }

    heightmap operator()(random_engine& engine, size_type width, size_type height) const;

  private:
    size_type m_count;
    double m_radius_min;
    double m_radius_max;

  };


}

#endif // MM_HILS_H
