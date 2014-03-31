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
#include <mm/biomize.h>

namespace mm {

  colormap biomize::operator()(const tilemap& src, const tileset& set) const {
    colormap::size_type width = src.width();
    colormap::size_type height = src.height();

    if (m_kind == kind::DETAILED) {
      width *= 2;
      height *= 2;
    }

    colormap map(width, height);

    for (auto x : src.x_range()) {
      for (auto y : src.y_range()) {
        if (m_kind == kind::SIMPLE) {
          int biome = src(x, y).biome();
          map(x, y) = set.biome_representation(biome);
        } else {
          int biome_nw = src(x, y).biome(tile::detail::NW);
          map(2*x, 2*y) = set.biome_representation(biome_nw);
          int biome_ne = src(x, y).biome(tile::detail::NE);
          map(2*x + 1, 2*y) = set.biome_representation(biome_ne);
          int biome_sw = src(x, y).biome(tile::detail::SW);
          map(2*x, 2*y + 1) = set.biome_representation(biome_sw);
          int biome_se = src(x, y).biome(tile::detail::SE);
          map(2*x + 1, 2*y + 1) = set.biome_representation(biome_se);
        }
      }
    }

    return map;
  }

}
