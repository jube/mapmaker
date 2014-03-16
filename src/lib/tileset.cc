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
#include <mm/tileset.h>

#include <mm/biomize.h>
#include <mm/tilemap.h>

namespace mm {

  bool tileset::add_terrain(const biome& biome) {
    auto ret = m_terrains.emplace(m_terrain_id++, biome);
    return ret.second;
  }

  int tileset::compute_biome(double altitude, double humidity, bool water) const {
    for (auto value : m_terrains) {
      if (value.second.match(altitude, humidity, water)) {
        return value.first;
      }
    }

    return -1;
  }

  color tileset::biome_representation(int biome) const {
    auto ret = m_terrains.find(biome);

    if (ret == m_terrains.end()) {
      return color::black();
    }

    return ret->second.representation();
  }

#define TILESET_SIZE 256

  void tileset::output_to_ppm(std::ostream& file) const {
    tilemap map(2 * TILESET_SIZE, TILESET_SIZE);

    for (tilemap::size_type x = 0; x < TILESET_SIZE; ++x) {
      for (tilemap::size_type y = 0; y < TILESET_SIZE; ++y) {
        double altitude = 1.0 - static_cast<double>(y) / TILESET_SIZE;
        double humidity = static_cast<double>(x) / TILESET_SIZE;

        map(x, y).set_biome(this->compute_biome(altitude, humidity, false));
        map(x + TILESET_SIZE, y).set_biome(this->compute_biome(altitude, humidity, true));
      }
    }

    auto colored = biomize()(map, *this);
    colored.output_to_ppm(file);
  }
}
