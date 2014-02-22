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
#ifndef MM_DIAMOND_SQUARE_H
#define MM_DIAMOND_SQUARE_H

#include <mm/random.h>
#include <mm/heightmap.h>

namespace mm {

  class diamond_square {
  public:
    typedef typename position::size_type size_type;

    diamond_square(double val = 0.0)
    : m_ne(val), m_nw(val), m_sw(val), m_se(val)
    {
    }

    diamond_square(double ne, double nw, double sw, double se)
    : m_ne(ne), m_nw(nw), m_sw(sw), m_se(se)
    {
    }

    heightmap operator()(random_engine& r, size_type width, size_type height) const;

  private:
    double m_ne;
    double m_nw;
    double m_sw;
    double m_se;
  };


}


#endif // MM_DIAMOND_SQUARE_H
