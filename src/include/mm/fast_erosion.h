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
#ifndef MM_FAST_EROSION_H
#define MM_FAST_EROSION_H

#include <mm/heightmap.h>

namespace mm {

  class fast_erosion {
  public:
    typedef std::size_t size_type;

    fast_erosion(size_type iterations, double talus, double fraction)
    : m_iterations(iterations), m_talus(talus), m_fraction(fraction)
    {
    }

    heightmap operator()(const heightmap& src) const;

  private:
    size_type m_iterations;
    double m_talus;
    double m_fraction;

  };

}

#endif // MM_FAST_EROSION_H
