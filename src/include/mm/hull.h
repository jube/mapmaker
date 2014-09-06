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
#ifndef MM_HULL_H
#define MM_HULL_H

#include <vector>

#include <mm/binarymap.h>

namespace mm {

  class hull {
  public:
    enum class angle_type {
      sharp,
      smooth,
    };

    hull(unsigned factor = 1, angle_type type = angle_type::sharp)
    : m_factor(factor)
    , m_type(type)
    {
    }

    std::vector<std::vector<position>> operator()(const binarymap& src) const;

  private:
    position transformed_position(const position& p) const {
      return { p.x * m_factor, p.y * m_factor };
    }

  private:
    unsigned m_factor;
    angle_type m_type;
  };

}

#endif // MM_HULL_H
