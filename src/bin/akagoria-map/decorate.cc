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
#include "decorate.h"

#include <cassert>
#include <iostream>
#include <queue>

#include <mm/binarymap.h>
#include <mm/utils.h>


namespace mm {

  namespace {

    class altitude_compare {
    public:
      typedef position value_type;

      altitude_compare(const heightmap& map)
      : m_map(map)
      {
      }

      bool operator()(const position& lhs, const position& rhs) {
        return m_map(lhs) > m_map(rhs);
      }

    private:
      const heightmap& m_map;
    };

    enum class type {
      SEA,
      RIVER,
      BANK,
      GROUND,
    };

  }

  static const int directions[8][2] = {
    //
    {  1,  1 },
    {  1, -1 },
    {  1,  1 },
    { -1,  1 },
    //
    {  0,  1 },
    {  0, -1 },
    {  1,  0 },
    { -1,  0 },
  };

  std::vector<position> generate_river(const heightmap& map, const binarymap& watermap, const position& source) {
    planemap<type> typemap(size_only, watermap);

    for (auto fp : watermap.positions()) {
      if (watermap(fp)) {
        typemap(fp) = type::SEA;
      } else {
        typemap(fp) = type::GROUND;
      }
    }

    altitude_compare compare(map);
    std::priority_queue<position, std::vector<position>, altitude_compare> queue(compare);
    queue.push(source);

    std::vector<position> river;

    position current = source;
    while (typemap(current) != type::SEA) {
      queue.pop();
      river.push_back(current);

      typemap(current) = type::RIVER;

      for (int k = 0; k < 8; ++k) {
        int i = directions[k][0];
        int j = directions[k][1];

        if (current.x == 0 && i == -1) {
          continue;
        }

        if (current.x == map.width() - 1 && i == 1) {
          continue;
        }

        if (current.y == 0 && j == -1) {
          continue;
        }

        if (current.y == map.height() - 1 && j == 1) {
          continue;
        }

        position next{ current.x + i, current.y +j };

        if (typemap(next) == type::GROUND) {
          typemap(next) = type::BANK;
          queue.push(next);
        } else if (typemap(next) == type::SEA) {
          queue.push(next);
        }
      }

      current = queue.top();
    }

    return river;
  }

  static std::vector<std::vector<position>> generate_rivers(const heightmap& map, const binarymap& watermap, unsigned rivers_count, double min_source_altitude, random_engine& engine) {
    std::uniform_int_distribution<tilemap::size_type> dist_x(0, map.width() - 1);
    std::uniform_int_distribution<tilemap::size_type> dist_y(0, map.height() - 1);

    std::vector<std::vector<position>> rivers;

    for (unsigned i = 0; i < rivers_count; ++i) {
      tilemap::size_type x, y;

      do {
        x = dist_x(engine);
        y = dist_y(engine);
      } while (map(x, y) < min_source_altitude);

      auto river = generate_river(map, watermap, {x, y});
      rivers.push_back(std::move(river));
    }

    return rivers;
  }


  /*
   * humidity
   */

  static heightmap compute_humiditymap(const binarymap& watermap) {
    heightmap humiditymap(size_only, watermap);

    std::queue<position> queue;

    for(auto x : watermap.x_range()) {
      for(auto y : watermap.y_range()) {
        if (watermap(x, y)) {
          humiditymap(x, y) = 0.9;
          queue.push({ x, y });
        }
      }
    }

    binarymap computed(watermap);

    while (!queue.empty()) {
      auto here = queue.front();
      assert(computed(here));

      double humidity_here = humiditymap(here);

      humiditymap.visit8neighbours(here, [humidity_here, &computed, &queue](position there, double& humidity_there) {
        if (computed(there)) {
          return;
        }

        humidity_there = std::pow(humidity_here, 1.05);
        computed(there) = true;
        queue.push(there);
      });

      queue.pop();
    }

    return humiditymap;
  }

  static binarymap compute_initial_watermap(const heightmap& src, double sea_level) {
    binarymap watermap(size_only, src);

    for (auto fp : src.positions()) {
      watermap(fp) = (src(fp) < sea_level);
    }

    return watermap;
  }

  static int compute_detailed_biome(int biome, int corner, int other_corner, const biomeset& set) {
    if (corner != other_corner) {
      return biome;
    }

    if (set.has_higher_priority(biome, corner)) {
      return biome;
    }

    return corner;
  }

  static void compute_detailed_tiles(tilemap& map, const biomeset& set) {
    for (auto x : map.x_range()) {
      for (auto y : map.y_range()) {
        int biome = map(x, y).biome();

        if (x > 0 && y > 0) {
          int detailed_biome = compute_detailed_biome(biome, map(x - 1, y).biome(), map(x, y - 1).biome(), set);
          map(x, y).set_biome(tile::detail::NW, detailed_biome);
        }

        if (x < map.height() - 1 && y > 0) {
          int detailed_biome = compute_detailed_biome(biome, map(x + 1, y).biome(), map(x, y - 1).biome(), set);
          map(x, y).set_biome(tile::detail::NE, detailed_biome);
        }

        if (x > 0 && y < map.width() - 1) {
          int detailed_biome = compute_detailed_biome(biome, map(x - 1, y).biome(), map(x, y + 1).biome(), set);
          map(x, y).set_biome(tile::detail::SW, detailed_biome);
        }

        if (x < map.height() - 1 && y < map.width() - 1) {
          int detailed_biome = compute_detailed_biome(biome, map(x + 1, y).biome(), map(x, y + 1).biome(), set);
          map(x, y).set_biome(tile::detail::SE, detailed_biome);
        }
      }
    }
  }


  tilemap decorate::operator()(const heightmap& src, const biomeset& set, random_engine& engine) const {
    binarymap watermap = compute_initial_watermap(src, m_sea_level);

    auto rivers = generate_rivers(src, watermap, m_rivers, m_min_source_altitude, engine);
    for (auto river : rivers) {
      for (auto water : river) {
        watermap(water) = true;
      }
    }

    heightmap humiditymap = compute_humiditymap(watermap);
    humiditymap.output_to_pgm("humidity.pnm");

    tilemap map(size_only, src);

    for (auto fp : src.positions()) {
      int biome = set.compute_biome(value_with_sea_level(src(fp), m_sea_level), humiditymap(fp), watermap(fp));
      map(fp).set_biome(biome);
    }

    compute_detailed_tiles(map, set);

    return map;
  }

}
