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
#ifndef MM_DECORATE_H
#define MM_DECORATE_H

#include <mm/heightmap.h>
#include <mm/random.h>
#include <mm/tilemap.h>
#include <mm/biomeset.h>

namespace mm {

  class decorate {
  public:
    decorate(double sea_level, unsigned rivers, double min_source_altitude)
    : m_sea_level(sea_level)
    , m_rivers(rivers)
    , m_min_source_altitude(min_source_altitude)
    {
    }

    tilemap operator()(const heightmap& src, const biomeset& set, random_engine& engine) const;

  private:
    double m_sea_level;
    unsigned m_rivers;
    double m_min_source_altitude;
  };

}

#endif // MM_DECORATE_H
