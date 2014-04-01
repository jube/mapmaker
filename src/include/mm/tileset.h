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
#ifndef MM_TILESET_H
#define MM_TILESET_H

#include <iosfwd>
#include <map>

#include <mm/tile.h>

namespace mm {

  class tileset {
  public:
    tileset(int first_gid)
    : m_first_gid(first_gid)
    , m_tile_id(0)
    {
    }

    typedef std::map<int, tile>::const_iterator const_iterator;

    const_iterator begin() const {
      return m_tiles.begin();
    }

    const_iterator end() const {
      return m_tiles.end();
    }

    std::size_t size() const {
      return m_tiles.size();
    }

    int compute_id(const tile& terrain);
    int compute_biome_id(int biome);

  private:
    int m_first_gid;
    int m_tile_id;
    std::map<int, tile> m_tiles;
  };

}

#endif // MM_TILESET_H
