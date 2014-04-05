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
#ifndef MM_PLAYABILITY_H
#define MM_PLAYABILITY_H

#include <tuple>

#include <mm/binarymap.h>
#include <mm/heightmap.h>
#include <mm/reachability.h>

namespace mm {

  class playability {
  public:
    playability(double sea_level, reachability::size_type unit_size, reachability::size_type building_size, double unit_talus, double building_talus, bool output_intermediates)
    : m_sea_level(sea_level)
    , m_unit_size(unit_size)
    , m_building_size(building_size)
    , m_unit_talus(unit_talus)
    , m_building_talus(building_talus)
    , m_output_intermediates(output_intermediates)
    {
    }

    std::tuple<binarymap, binarymap, binarymap> operator()(const heightmap& src) const;

  private:
      double m_sea_level;
      reachability::size_type m_unit_size;
      reachability::size_type m_building_size;
      double m_unit_talus;
      double m_building_talus;
      bool m_output_intermediates;
  };

}

#endif // MM_PLAYABILITY_H
