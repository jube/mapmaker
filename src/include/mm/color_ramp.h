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
#ifndef MM_COLOR_RAMP_H
#define MM_COLOR_RAMP_H

#include <map>

#include <mm/color.h>

namespace mm {

  class color_ramp {
  public:
    color_ramp()
    : m_min(0.0), m_max(0.0)
    {
    }

    bool empty() const {
      return m_map.empty();
    }

    void add_color_stop(double offset, const color& c);

    color compute_color(double offset) const;

  private:
    double m_min;
    double m_max;
    std::map<double, color> m_map;
  };

}

#endif // MM_COLOR_RAMP_H
