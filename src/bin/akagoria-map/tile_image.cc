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
#include "tile_image.h"

#define EXT_TILE_SIZE (TILE_SIZE + 2)

namespace mm {

  colormap tile_image::operator()(const tileset& tiles, const biomeset& set) const {
    std::size_t tiles_count = tiles.size();

    std::size_t dim = 1;

    while (dim * dim < tiles_count) {
      dim += 1;
    }

    std::size_t size = dim * EXT_TILE_SIZE;
    colormap map(size, size);

    std::size_t idx = 0;

    for (auto value : tiles) {
      std::size_t i = idx % dim;
      std::size_t x0 = i * EXT_TILE_SIZE;
      std::size_t x2 = x0 + EXT_TILE_SIZE;
      std::size_t x1 = x0 + (x2 - x0) / 2;

      std::size_t j = idx / dim;
      std::size_t y0 = j * EXT_TILE_SIZE;
      std::size_t y2 = y0 + EXT_TILE_SIZE;
      std::size_t y1 = y0 + (y2 - y0) / 2;

      auto tile = value.second;

      {
        int biome = tile.biome(tile::detail::NW);
        color rep = set.biome_representation(biome);
        for (auto x = x0; x < x1; x++) {
          for (auto y = y0; y < y1; y++) {
            map(x, y) = rep;
          }
        }
      }
      {
        int biome = tile.biome(tile::detail::NE);
        color rep = set.biome_representation(biome);
        for (auto x = x1; x < x2; x++) {
          for (auto y = y0; y < y1; y++) {
            map(x, y) = rep;
          }
        }
      }
      {
        int biome = tile.biome(tile::detail::SW);
        color rep = set.biome_representation(biome);
        for (auto x = x0; x < x1; x++) {
          for (auto y = y1; y < y2; y++) {
            map(x, y) = rep;
          }
        }
      }
      {
        int biome = tile.biome(tile::detail::SE);
        color rep = set.biome_representation(biome);
        for (auto x = x1; x < x2; x++) {
          for (auto y = y1; y < y2; y++) {
            map(x, y) = rep;
          }
        }
      }

      idx++;
    }

    return map;
  }

}
