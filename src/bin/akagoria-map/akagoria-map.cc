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
#include <cinttypes>
#include <cstdio>
#include <exception>
#include <queue>
#include <stdexcept>

#include <yaml-cpp/yaml.h>

#include <mm/color.h>
#include <mm/heightmap.h>
#include <mm/playability.h>
#include <mm/random.h>
#include <mm/reachability.h>
#include <mm/utils.h>

namespace {

  /*
   * biomes
   */

  struct range {
    double min;
    double max;
  };

  struct climate {
    range altitude;
    range humidity;
    bool water;
  };

  static bool climate_match(const climate& c, double  altitude, double humidity, bool water) {
    if (altitude < c.altitude.min || c.altitude.max < altitude) {
      return false;
    }

    if (humidity < c.humidity.min || c.humidity.max < humidity) {
      return false;
    }

    return water == c.water;
  }

  class biome {
  public:
    biome(const std::string& name, const mm::color& rep)
    : m_name(name)
    , m_rep(rep)
    {
    }

    const std::string& name() const {
      return m_name;
    }

    mm::color representation() const {
      return m_rep;
    }

    biome& add_climate(range altitude_range, range humidity_range, bool water) {
      m_climates.emplace_back(climate{altitude_range, humidity_range, water});
      return *this;
    }

    bool match(double altitude, double humidity, bool water) const {
      for (auto climate : m_climates) {
        if (climate_match(climate, altitude, humidity, water)) {
          return true;
        }
      }

      return false;
    }

    bool has_higher_priority(const biome& other) const {
      return false;
    }

  private:
    std::string m_name;
    mm::color m_rep;
    std::vector<climate> m_climates;
  };

  class biomeset {
  public:
    biomeset()
    : m_terrain_id(0)
    {
    }

    biome& add_terrain(const biome& biome) {
      auto ret = m_terrains.emplace(m_terrain_id++, biome);
      return ret.first->second;
    }

    int get_next_id() const {
      return m_terrain_id;
    }

    int compute_biome(double altitude, double humidity, bool water) const {
      for (auto value : m_terrains) {
        if (value.second.match(altitude, humidity, water)) {
          return value.first;
        }
      }

      return -1;
    }

    bool has_higher_priority(int biome_id, int other_biome_id) const  {
      auto it = m_terrains.find(biome_id);
      assert(it != m_terrains.end());

      auto it_other = m_terrains.find(other_biome_id);
      assert(it_other != m_terrains.end());

      auto biome = it->second;
      auto other_biome = it_other->second;

      return biome.has_higher_priority(other_biome);
    }

    const std::string& name(int biome) const {
      static const std::string undef = "Undefined";

      auto ret = m_terrains.find(biome);

      if (ret == m_terrains.end()) {
        return undef;
      }

      return ret->second.name();
    }

    mm::color biome_representation(int biome) const {
      auto ret = m_terrains.find(biome);

      if (ret == m_terrains.end()) {
        return mm::color::black();
      }

      return ret->second.representation();
    }

#define TILESET_SIZE 256

#if 0
    void output_to_ppm(std::ostream& file) const {
      tilemap map(2 * TILESET_SIZE, TILESET_SIZE);

      for (tilemap::size_type x = 0; x < TILESET_SIZE; ++x) {
        for (tilemap::size_type y = 0; y < TILESET_SIZE; ++y) {
          double altitude = 1.0 - static_cast<double>(y) / TILESET_SIZE;
          double humidity = static_cast<double>(x) / TILESET_SIZE;

          map(x, y).set_biome(this->compute_biome(altitude, humidity, false));
          map(x + TILESET_SIZE, y).set_biome(this->compute_biome(altitude, humidity, true));
        }
      }

//       auto colored = biomize(biomize::kind::SIMPLE)(map, *this);
//       colored.output_to_ppm(file);
    }

    void output_to_ppm(const std::string& filename) const {
      std::ofstream file(filename);
      return output_to_ppm(file);
    }
#endif

    static biomeset whittaker() {
      biomeset set;
      /*
       * water
       */
      set.add_terrain({ "Water", { 0x33, 0x66, 0x99 }})
          .add_climate({ 0, 1 }, { 0, 1 }, true);
      /*
       * non-water
       */
      set.add_terrain({ "Beach", { 0xA0, 0x90, 0x77 }})
          .add_climate({ 0.5, 0.52 }, { 0, 1 }, false);
      // low altitudes
      set.add_terrain({ "Subtropical Desert", { 0xD2, 0xB9, 0x8B }})
          .add_climate({ 0.52, 0.65 }, { 0, 0.16 }, false);
      set.add_terrain({ "Grassland", { 0x88, 0xAA, 0x55 }})
          .add_climate({ 0.52, 0.65 }, { 0.16, 0.33 }, false)
          .add_climate({ 0.65, 0.8 }, { 0.16, 0.5 }, false);
      set.add_terrain({ "Tropical Seasonal Forest", { 0x55, 0x99, 0x44 }})
          .add_climate({ 0.52, 0.65 }, { 0.33, 0.66 }, false);
      set.add_terrain({ "Tropical Rain Forest", { 0x33, 0x77, 0x55 }})
          .add_climate({ 0.52, 0.65 }, { 0.66, 1 }, false);
      // mid altitudes
      set.add_terrain({ "Temperate Desert", { 0xC9, 0xD2, 0x9B }})
          .add_climate({ 0.65, 0.8 }, { 0, 0.16 }, false)
          .add_climate({ 0.8, 0.9 }, { 0, 0.33 }, false);
      set.add_terrain({ "Temperate Deciduous Forest", { 0x67, 0x94, 0x59 }})
          .add_climate({ 0.65, 0.8 }, { 0.5, 0.83 }, false);
      set.add_terrain({ "Temperate Rain Forest", { 0x44, 0x88, 0x55 }})
          .add_climate({ 0.65, 0.8 }, { 0.83, 1 }, false);
      // high altitudes
      set.add_terrain({ "Shrubland", { 0x88, 0x99, 0x77 }})
          .add_climate({ 0.8, 0.9 }, { 0.33, 0.66 }, false);
      set.add_terrain({ "Taiga", { 0x99, 0xAA, 0x77 }})
          .add_climate({ 0.8, 0.9 }, { 0.66, 1 }, false);
      // very high altitudes
      set.add_terrain({ "Scorched", { 0x55, 0x55, 0x55 }})
          .add_climate({ 0.9, 1 }, { 0, 0.16 }, false);
      set.add_terrain({ "Bare", { 0x88, 0x88, 0x88 }})
          .add_climate({ 0.9, 1 }, { 0.16, 0.33 }, false);
      set.add_terrain({ "Tundra", { 0xBB, 0xBB, 0xAA }})
          .add_climate({ 0.9, 1 }, { 0.33, 0.5 }, false);
      set.add_terrain({ "Snow", { 0xFF, 0xFF, 0xFF }})
          .add_climate({ 0.9, 1 }, { 0.5, 1 }, false);

      return set;
    }

  private:
    int m_terrain_id;
    std::map<int, biome> m_terrains;
  };

  /*
   * tiles
   */

#define TILE_N 0
#define TILE_S 1
#define TILE_W 0
#define TILE_E 1

  class tile {
  public:

    enum class detail {
      NW,
      NE,
      SW,
      SE,
    };

    int biome(detail where) const {
      switch (where) {
        case detail::NW:
          return m_details[TILE_N][TILE_W];
        case detail::NE:
          return m_details[TILE_N][TILE_E];
        case detail::SW:
          return m_details[TILE_S][TILE_W];
        case detail::SE:
          return m_details[TILE_S][TILE_E];
      }

      return -1;
    }

    void set_biome(detail where, int biome) {
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

  private:
    int m_details[2][2] = { { -1, -1 }, { -1, -1 } };
  };

  static bool operator==(const tile& left, const tile& right) {
    for (auto where : { tile::detail::NW, tile::detail::NE, tile::detail::SW, tile::detail::SE }) {
      if (left.biome(where) != right.biome(where)) {
        return false;
      }
    }

    return true;
  }

  class tilemap : public mm::planemap<tile> {
  public:
    typedef typename mm::planemap<tile>::value_type value_type;
    typedef typename mm::planemap<tile>::allocator_type allocator_type;
    typedef typename mm::planemap<tile>::size_type size_type;
    typedef typename mm::planemap<tile>::difference_type difference_type;
    typedef typename mm::planemap<tile>::reference reference;
    typedef typename mm::planemap<tile>::const_reference const_reference;
    typedef typename mm::planemap<tile>::pointer pointer;
    typedef typename mm::planemap<tile>::const_pointer const_pointer;

    tilemap()
    {
    }

    tilemap(size_type w, size_type h)
    : mm::planemap<tile>(w, h)
    {
    }

    tilemap(size_type w, size_type h, tile value)
    : mm::planemap<tile>(w, h, value)
    {
    }

    tilemap(const tilemap& other)
    : mm::planemap<tile>(other)
    {
    }

    template<typename T>
    tilemap(mm::size_only_t, const mm::planemap<T>& other)
    : tilemap(other.width(), other.height())
    {
    }

    tilemap(tilemap&& other)
    : mm::planemap<tile>(other)
    {
    }

    tilemap& operator=(const tilemap& other) {
      if (this == &other) {
        return *this;
      }

      mm::planemap<tile>::operator=(other);
      return *this;
    }

    tilemap& operator=(tilemap&& other) {
      if (this == &other) {
        return *this;
      }

      mm::planemap<tile>::operator=(other);
      return *this;
    }

    // specialized methods
  };

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

    int compute_id(const tile& terrain) {
      for (auto value : m_tiles) {
        if (terrain == value.second) {
          return value.first + m_first_gid;
        }
      }

      int id = m_tile_id++;
      m_tiles.emplace(id, terrain);

      return id + m_first_gid;
    }

    int compute_biome_id(int biome) {
      tile dummy;
      dummy.set_biome(tile::detail::NW, biome);
      dummy.set_biome(tile::detail::NE, biome);
      dummy.set_biome(tile::detail::SW, biome);
      dummy.set_biome(tile::detail::SE, biome);
      return compute_id(dummy);
    }

  private:
    int m_first_gid;
    int m_tile_id;
    std::map<int, tile> m_tiles;
  };



  /*
   * misc
   */

  class altitude_compare {
  public:
    typedef mm::position value_type;

    altitude_compare(const mm::heightmap& map)
    : m_map(map)
    {
    }

    bool operator()(const mm::position& lhs, const mm::position& rhs) {
      return m_map(lhs) > m_map(rhs);
    }

  private:
    const mm::heightmap& m_map;
  };

  enum class type {
    SEA,
    RIVER,
    BANK,
    GROUND,
  };
}


/*
 * rivers
 */

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

static std::vector<mm::position> generate_river(const mm::heightmap& map, const mm::binarymap& watermap, const mm::position& source) {
  mm::planemap<type> typemap(mm::size_only, watermap);

  for (auto fp : watermap.positions()) {
    if (watermap(fp)) {
      typemap(fp) = type::SEA;
    } else {
      typemap(fp) = type::GROUND;
    }
  }

  altitude_compare compare(map);
  std::priority_queue<mm::position, std::vector<mm::position>, altitude_compare> queue(compare);
  queue.push(source);

  std::vector<mm::position> river;

  mm::position current = source;
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

      mm::position next{ current.x + i, current.y +j };

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

static std::vector<std::vector<mm::position>> generate_rivers(const mm::heightmap& map, const mm::binarymap& watermap, unsigned rivers_count, double rivers_min_source_altitude, mm::random_engine& engine) {
  std::uniform_int_distribution<mm::heightmap::size_type> dist_x(0, map.width() - 1);
  std::uniform_int_distribution<mm::heightmap::size_type> dist_y(0, map.height() - 1);

  std::vector<std::vector<mm::position>> rivers;

  for (unsigned i = 0; i < rivers_count; ++i) {
    mm::heightmap::size_type x, y;

    do {
      x = dist_x(engine);
      y = dist_y(engine);
    } while (map(x, y) < rivers_min_source_altitude);

    auto river = generate_river(map, watermap, {x, y});
    rivers.push_back(std::move(river));
  }

  return rivers;
}

/*
 * humidity
 */

static mm::heightmap compute_humiditymap(const mm::binarymap& watermap) {
  mm::heightmap humiditymap(mm::size_only, watermap);

  std::queue<mm::position> queue;

  for(auto x : watermap.x_range()) {
    for(auto y : watermap.y_range()) {
      if (watermap(x, y)) {
        humiditymap(x, y) = 0.9;
        queue.push({ x, y });
      }
    }
  }  class tileset {
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


  mm::binarymap computed(watermap);

  while (!queue.empty()) {
    auto here = queue.front();
    assert(computed(here));

    double humidity_here = humiditymap(here);

    humiditymap.visit8neighbours(here, [humidity_here, &computed, &queue](mm::position there, double& humidity_there) {
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

static mm::binarymap compute_initial_watermap(const mm::heightmap& src, double sea_level) {
  mm::binarymap watermap(mm::size_only, src);

  for (auto fp : src.positions()) {
    watermap(fp) = (src(fp) < sea_level);
  }

  return watermap;
}

/*
 * tiles
 */
static tilemap compute_tilemap(const mm::planemap<int>& biomemap) {
  assert(biomemap.width() > 2);
  assert(biomemap.height() > 2);

  tilemap::size_type width = biomemap.width() - 1;
  tilemap::size_type height = biomemap.height() - 1;

  tilemap map(width, height);

  for (auto x : map.x_range()) {
    for (auto y : map.y_range()) {
      map(x, y).set_biome(tile::detail::NW, biomemap(x,     y));
      map(x, y).set_biome(tile::detail::NE, biomemap(x + 1, y));
      map(x, y).set_biome(tile::detail::SW, biomemap(x,     y + 1));
      map(x, y).set_biome(tile::detail::SE, biomemap(x + 1, y + 1));
    }
  }

  return map;
}

class bad_structure : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};


void generate_akagoria_map(YAML::Node node) {
  /*
   * get parameters
   */
  mm::random_engine::result_type seed = 0;

  auto seed_node = node["seed"];
  if (seed_node) {
    seed = seed_node.as<mm::random_engine::result_type>();
  } else {
    std::random_device dev;
    seed = dev();
    std::printf("Using 'random_device' for seed: %" PRIuFAST64 "\n", seed);
  }
  mm::random_engine engine(seed);

  auto heightmap_node = node["heightmap"];
  if (!heightmap_node) {
    throw bad_structure("akagoria-map: missing 'heightmap' in parameters");
  }

  auto sea_level_node = node["sea_level"];
  if (!sea_level_node) {
    throw bad_structure("mapmaker: missing 'sea_level' in 'tiled' output parameters");
  }
  auto sea_level = sea_level_node.as<double>();

  auto unit_size_node = node["unit_size"];
  if (!unit_size_node) {
    throw bad_structure("mapmaker: missing 'unit_size' in 'tiled' output parameters");
  }
  auto unit_size = unit_size_node.as<mm::reachability::size_type>();

  auto unit_talus_node = node["unit_talus"];
  if (!unit_talus_node) {
    throw bad_structure("mapmaker: missing 'unit_talus' in 'tiled' output parameters");
  }
  auto unit_talus = unit_talus_node.as<double>();

  auto rivers_count_node = node["rivers_count"];
  if (!rivers_count_node) {
    throw bad_structure("mapmaker: missing 'rivers' in 'tiled' output parameters");
  }
  auto rivers_count = rivers_count_node.as<unsigned>();

  auto rivers_min_source_altitude_node = node["rivers_min_source_altitude"];
  if (!rivers_min_source_altitude_node) {
    throw bad_structure("mapmaker: missing 'min_source_altitude' in 'tiled' output parameters");
  }
  auto rivers_min_source_altitude = rivers_min_source_altitude_node.as<double>();


  /*
   * load map and add decorations
   */
  mm::heightmap map = mm::heightmap::input_from_pgm(heightmap_node.as<std::string>());

  // rivers
  auto watermap = compute_initial_watermap(map, sea_level);

  auto rivers = generate_rivers(map, watermap, rivers_count, rivers_min_source_altitude, engine);
  for (auto river : rivers) {
    for (auto water : river) {
      watermap(water) = true;
    }
  }

  // humidity
  auto humiditymap = compute_humiditymap(watermap);
  humiditymap.output_to_pgm("humidity.pnm");

  // biomes
  biomeset set = biomeset::whittaker();
  mm::planemap<int> biomemap(mm::size_only, map);

  for (auto fp : map.positions()) {
    biomemap(fp) = set.compute_biome(mm::value_with_sea_level(map(fp), sea_level), humiditymap(fp), watermap(fp));
  }

  // tilemap
  auto tilemap = compute_tilemap(biomemap);

  // unit_map
  mm::heightmap::size_type size_max, size_min;
  std::tie(size_min, size_max) = std::minmax(map.width(), map.height());
  auto size = size_min + (size_max - size_min) / 2; // to avoid overflow

  mm::binarymap unit_map;
  std::tie(std::ignore, unit_map, std::ignore) =
      mm::playability(sea_level, unit_size, unit_size, unit_talus / size, unit_talus / size, false)(map);



}

static void usage() {
  std::printf("Usage: mapmaker <file>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage();
    return 0;
  }

  try {
    YAML::Node node = YAML::LoadFile(argv[1]);
    generate_akagoria_map(node);

  } catch (std::exception& ex) {
    std::printf("Error: %s\n", ex.what());
    return 1;
  }

  return 0;
}
