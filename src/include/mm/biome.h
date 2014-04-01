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
#ifndef MM_BIOME_H
#define MM_BIOME_H

#include <string>

#include <mm/color.h>

namespace mm {

  struct range {
    double min;
    double max;
  };

  class biome {
  public:
    biome(const std::string& name, const color& rep, range altitude_range, range humidity_range, bool water)
    : m_name(name)
    , m_rep(rep)
    , m_altitude_range(altitude_range)
    , m_humidity_range(humidity_range)
    , m_water(water)
    {
    }

    const std::string& name() const {
      return m_name;
    }

    color representation() const {
      return m_rep;
    }

    bool match(double altitude, double humidity, bool water) const;

    bool has_higher_priority(const biome& other) const;

  private:
    std::string m_name;
    color m_rep;
    range m_altitude_range;
    range m_humidity_range;
    bool m_water;
  };

}

#endif // MM_BIOME_H
