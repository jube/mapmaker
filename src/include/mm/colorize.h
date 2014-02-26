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
#ifndef MM_COLORIZE_H
#define MM_COLORIZE_H

#include <mm/colormap.h>
#include <mm/color_ramp.h>
#include <mm/heightmap.h>

namespace mm {

  class colorize {
  public:
    typedef std::size_t size_type;

    colorize(color_ramp ramp, double sea_level)
    : m_ramp(ramp)
    , m_sea_level(sea_level)
    {
    }

    colormap operator()(const heightmap& map) const;

  private:
    color_ramp m_ramp;
    double m_sea_level;
  };


}

#endif // MM_COLORIZE_H
