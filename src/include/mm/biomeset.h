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
#ifndef MM_BIOMESET_H
#define MM_BIOMESET_H

#include <iosfwd>
#include <map>

#include <mm/biome.h>

namespace mm {

  class biomeset {
  public:
    biomeset()
    : m_terrain_id(0)
    {
    }

    bool add_terrain(const biome& biome);

    int compute_biome(double altitude, double humidity, bool water) const;
    bool has_higher_priority(int biome_id, int other_biome_id) const;

    color biome_representation(int biome) const;

    void output_to_ppm(std::ostream& file) const;

    static biomeset whittaker();

  private:
    int m_terrain_id;
    std::map<int, biome> m_terrains;
  };

}

#endif // MM_BIOMESET_H
