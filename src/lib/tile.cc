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
#include <mm/tile.h>

#define TILE_N 0
#define TILE_S 1
#define TILE_W 0
#define TILE_E 1

namespace mm {

  void tile::set_biome(int biome) {
    m_biome = biome;
    m_details[TILE_N][TILE_W] = biome;
    m_details[TILE_N][TILE_E] = biome;
    m_details[TILE_S][TILE_W] = biome;
    m_details[TILE_S][TILE_E] = biome;
  }

  void tile::set_biome(detail where, int biome) {
    switch (where) {
      case detail::NW:
        m_details[TILE_N][TILE_W] = biome;
        break;

      case detail::NE:
        m_details[TILE_N][TILE_E] = biome;
        break;

      case detail::SW:
        m_details[TILE_S][TILE_W] = biome;
        break;

      case detail::SE:
        m_details[TILE_S][TILE_E] = biome;
        break;
    }
  }

}
