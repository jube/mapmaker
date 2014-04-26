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
#include <mm/playability.h>

#include <mm/accessibility.h>
#include <mm/cutoff.h>
#include <mm/logical_combine.h>
#include <mm/slope.h>


namespace mm {

  std::tuple<binarymap, binarymap, binarymap> playability::operator()(const heightmap& src) const {
    auto island_map = cutoff(m_sea_level)(src);

    auto slope_map = slope()(src);

    // unit map
    auto unit_map = cutoff(m_unit_talus)(slope_map);
    unit_map = logical_combine()(unit_map, island_map, [](bool lhs, bool rhs) { return lhs & !rhs; });

    if (m_output_intermediates) {
      unit_map.output_to_pbm("unit1.pnm");
    }

    unit_map = reachability(m_unit_size)(unit_map);

    if (m_output_intermediates) {
      unit_map.output_to_pbm("unit2.pnm");
    }

    unit_map = accessibility()(unit_map);

    if (m_output_intermediates) {
      unit_map.output_to_pbm("unit3.pnm");
    }

    // building map
    auto building_map = cutoff(m_building_talus)(slope_map);
    building_map = logical_combine()(building_map, island_map, [](bool lhs, bool rhs) { return lhs & !rhs; });

    if (m_output_intermediates) {
      building_map.output_to_pbm("building1.pnm");
    }

    building_map = reachability(m_building_size)(building_map);

    if (m_output_intermediates) {
      building_map.output_to_pbm("building2.pnm");
    }

    building_map = logical_combine()(building_map, unit_map, std::logical_and<bool>());

    if (m_output_intermediates) {
      building_map.output_to_pbm("building3.pnm");
    }

    return std::make_tuple(std::move(island_map), std::move(unit_map), std::move(building_map));
  }

}
