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
#include "tileset.h"

#include <cassert>
#include <iostream>

namespace mm {
  static bool are_tiles_equal(const tile& left, const tile& right) {
    for (auto where : { tile::detail::NW, tile::detail::NE, tile::detail::SW, tile::detail::SE }) {
      if (left.biome(where) != right.biome(where)) {
        return false;
      }
    }

    return true;
  }

  int tileset::compute_id(const tile& terrain) {
    for (auto value : m_tiles) {
      if (are_tiles_equal(terrain, value.second)) {
        return value.first + m_first_gid;
      }
    }

    int id = m_tile_id++;
    m_tiles.emplace(id, terrain);

    return id + m_first_gid;
  }

  int tileset::compute_biome_id(int biome) {
    tile dummy;
    dummy.set_biome(biome);
    return compute_id(dummy);
  }

}
