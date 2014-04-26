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

#include <algorithm>
#include <exception>
#include <fstream>
#include <queue>
#include <stdexcept>

#include <yaml-cpp/yaml.h>

#include <mm/color.h>
#include <mm/colorize.h>
#include <mm/colormap.h>
#include <mm/heightmap.h>
#include <mm/hull.h>
#include <mm/playability.h>
#include <mm/random.h>
#include <mm/reachability.h>
#include <mm/shader.h>
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
      if (m_climates.front().water != other.m_climates.front().water) {
        return m_climates.front().water;
      }

      return m_climates.front().humidity.min < other.m_climates.front().humidity.min;
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

    std::map<int, std::string> terrains() const {
      std::map<int, std::string> all_terrains;

      for (auto terrain : m_terrains) {
        all_terrains.emplace(terrain.first, terrain.second.name());
      }

      return all_terrains;
    }

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
  }

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

static constexpr std::size_t TILE_SIZE = 32;
static constexpr std::size_t EXT_TILE_SIZE = TILE_SIZE + 2;
static constexpr std::size_t EXT_TILE_SIZE_2 = EXT_TILE_SIZE / 2;

static void draw_rectangle(mm::colormap& map, std::size_t x, std::size_t y, mm::color color) {
  for (std::size_t dx = 0; dx < EXT_TILE_SIZE_2; dx++) {
    for (std::size_t dy = 0; dy < EXT_TILE_SIZE_2; dy++) {
      map(x + dx, y + dy) = color;
    }
  }
}

static void draw_triangles_slash(mm::colormap& map, std::size_t x, std::size_t y, mm::color color_hi, mm::color color_lo, bool limit_is_high) {
  for (std::size_t dx = 0; dx < EXT_TILE_SIZE_2; dx++) {
    std::size_t stop = 0;
    if (limit_is_high) {
      stop = EXT_TILE_SIZE_2 - dx;
    } else {
      if (dx < EXT_TILE_SIZE_2 - 2) {
        stop = EXT_TILE_SIZE_2 - dx - 2;
      }
    }
    for (std::size_t dy = 0; dy < EXT_TILE_SIZE_2; dy++) {
      if (dy < stop) {
        map(x + dx, y + dy) = color_hi;
      } else {
        map(x + dx, y + dy) = color_lo;
      }
    }
  }
}

static void draw_triangles_backslash(mm::colormap& map, std::size_t x, std::size_t y, mm::color color_hi, mm::color color_lo, bool limit_is_high) {
  for (std::size_t dx = 0; dx < EXT_TILE_SIZE_2; dx++) {
    std::size_t stop = 0;
    if (limit_is_high) {
      stop = dx + 1;
    } else {
      if (dx > 1) {
        stop = dx - 1;
      }
    }
    for (std::size_t dy = 0; dy < EXT_TILE_SIZE_2; dy++) {
      if (dy < stop) {
        map(x + dx, y + dy) = color_hi;
      } else {
        map(x + dx, y + dy) = color_lo;
      }
    }
  }
}

mm::colormap compute_tileset_image(const tileset& tiles, const biomeset& set) {
  std::size_t tiles_count = tiles.size();

  std::size_t dim = 1;

  while (dim * dim < tiles_count) {
    dim += 1;
  }

  std::size_t size = dim * EXT_TILE_SIZE;
  mm::colormap map(size, size);

  std::size_t idx = 0;

  for (auto value : tiles) {
    std::size_t i = idx % dim;
    std::size_t x0 = i * EXT_TILE_SIZE;
    std::size_t x1 = x0 + EXT_TILE_SIZE_2;

    std::size_t j = idx / dim;
    std::size_t y0 = j * EXT_TILE_SIZE;
    std::size_t y1 = y0 + EXT_TILE_SIZE_2;

    auto tile = value.second;

    int biome_nw = tile.biome(tile::detail::NW);
    int biome_ne = tile.biome(tile::detail::NE);
    int biome_sw = tile.biome(tile::detail::SW);
    int biome_se = tile.biome(tile::detail::SE);

    auto color_nw = set.biome_representation(biome_nw);
    auto color_ne = set.biome_representation(biome_ne);
    auto color_sw = set.biome_representation(biome_sw);
    auto color_se = set.biome_representation(biome_se);

    // NW
    if (biome_ne == biome_sw) {
      int biome_anti = biome_ne;
      auto color_anti = color_ne;
      if (biome_nw != biome_se || !set.has_higher_priority(biome_nw, biome_anti)) {
        draw_triangles_slash(map, x0, y0, color_nw, color_anti, true);
      } else {
        draw_rectangle(map, x0, y0, color_nw);
      }
    } else {
      draw_rectangle(map, x0, y0, color_nw);
    }

    // SE
    if (biome_ne == biome_sw) {
      int biome_anti = biome_ne;
      auto color_anti = color_ne;
      if (biome_se != biome_nw || !set.has_higher_priority(biome_se, biome_anti)) {
        draw_triangles_slash(map, x1, y1, color_anti, color_se, false);
      } else {
        draw_rectangle(map, x1, y1, color_se);
      }
    } else {
      draw_rectangle(map, x1, y1, color_se);
    }

    // NE
    if (biome_nw == biome_se) {
      int biome_anti = biome_nw;
      auto color_anti = color_nw;
      if (biome_ne != biome_sw || !set.has_higher_priority(biome_ne, biome_anti)) {
        draw_triangles_backslash(map, x1, y0, color_ne, color_anti, true);
      } else {
        draw_rectangle(map, x1, y0, color_ne);
      }
    } else {
      draw_rectangle(map, x1, y0, color_ne);
    }

    // SW
    if (biome_nw == biome_se) {
      int biome_anti = biome_nw;
      auto color_anti = color_nw;
      if (biome_sw != biome_ne || !set.has_higher_priority(biome_sw, biome_anti)) {
        draw_triangles_backslash(map, x0, y1, color_anti, color_sw, false);
      } else {
        draw_rectangle(map, x0, y1, color_sw);
      }
    } else {
      draw_rectangle(map, x0, y1, color_sw);
    }

    idx++;
  }

  return map;
}

class bad_structure : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};


void generate_akagoria_map(YAML::Node node, std::string filename, std::string imgname) {
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


  bool output_intermediates = true;

  /*
   * load map
   */
  mm::heightmap map = mm::heightmap::input_from_pgm(heightmap_node.as<std::string>());

  /*
   * generate rivers
   */
  auto watermap = compute_initial_watermap(map, sea_level);

  auto rivers = generate_rivers(map, watermap, rivers_count, rivers_min_source_altitude, engine);
  for (auto river : rivers) {
    for (auto water : river) {
      watermap(water) = true;
    }
  }

  // display the original map with the rivers

  if (output_intermediates) {
    auto colored = mm::colorize(mm::color_ramp::basic(), sea_level)(map);

    for (auto river : rivers) {
      for (auto water : river) {
        colored(water) = { 0x44, 0x77, 0xAA }; // { 17,  82, 112 };
      }
    }

    auto shaded = mm::shader(sea_level)(colored, map);
    shaded.output_to_ppm("map_with_rivers.pnm");
  }

  /*
   * compute humidity
   */
  auto humiditymap = compute_humiditymap(watermap);

  if (output_intermediates) {
    humiditymap.output_to_pgm("humidity.pnm");
  }

  /*
   * compute biomes
   */
  biomeset set = biomeset::whittaker();

  if (output_intermediates) {
#define BIOMESET_SIZE 256
    mm::colormap biomes(BIOMESET_SIZE, BIOMESET_SIZE);

    for (auto x : biomes.x_range()) {
      double humidity = static_cast<double>(x) / BIOMESET_SIZE;
      for (auto y : biomes.y_range()) {
        double altitude = static_cast<double>(BIOMESET_SIZE - y) / BIOMESET_SIZE * 0.5 + 0.5;

        int biome = set.compute_biome(altitude, humidity, false);
        biomes(x, y) = set.biome_representation(biome);
      }
    }

    biomes.output_to_ppm("biomeset.pnm");
  }


  mm::planemap<int> biomemap(mm::size_only, map);

  for (auto fp : map.positions()) {
    biomemap(fp) = set.compute_biome(mm::value_with_sea_level(map(fp), sea_level), humiditymap(fp), watermap(fp));
    assert(biomemap(fp) != -1);
  }

  if (output_intermediates) {
    mm::colormap biomes(mm::size_only, biomemap);

    for (auto fp : biomes.positions()) {
      biomes(fp) = set.biome_representation(biomemap(fp));
    }

    biomes.output_to_ppm("map_of_biomes.pnm");

    biomes = mm::shader(sea_level)(biomes, map);
    biomes.output_to_ppm("map_of_biomes_shaded.pnm");
  }

  /*
   * compute the tilemap
   */
  auto tilemap = compute_tilemap(biomemap);

  /*
   * compute tileset
   */
#define FIRST_GID 1

  auto terrains = set.terrains();

  tileset tiles(FIRST_GID);
  for (auto terrain : terrains) {
    tiles.compute_biome_id(terrain.first);
  }

  std::vector<int> gids;
  for (auto fp : tilemap.positions()) {
    int gid = tiles.compute_id(tilemap(fp));
    assert(gid > 0);
    gids.push_back(gid);
  }

  auto img = compute_tileset_image(tiles, set);
  img.output_to_ppm(imgname);

  /*
   * compute the unit map
   */
  mm::heightmap::size_type size_max, size_min;
  std::tie(size_min, size_max) = std::minmax(map.width(), map.height());
  auto size = size_min + (size_max - size_min) / 2; // to avoid overflow

  mm::binarymap unit_map;
  std::tie(std::ignore, unit_map, std::ignore) =
      mm::playability(sea_level, unit_size, unit_size, unit_talus / size, unit_talus / size, false)(map);

  if (output_intermediates) {
    unit_map.output_to_pbm("unit_map.pnm");
  }

  /*
   * compute the tmx file
   */
  std::ofstream file(filename);
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file << "<map version=\"1.0\" orientation=\"orthogonal\" ";
  file << "width=\"" << tilemap.width() << "\" height=\"" << tilemap.height() << "\" ";
  file << "tilewidth=\"" << TILE_SIZE << "\" tileheight=\"" << TILE_SIZE << "\">\n";

  file << "<tileset firstgid=\"1\" name=\"Biomes\" ";
  file << "tilewidth=\"" << TILE_SIZE << "\" tileheight=\"" << TILE_SIZE << "\" spacing=\"2\" margin=\"1\">\n";

  file << "<image source=\"" << imgname << "\" width=\"" << img.width() << "\" height=\"" << img.height() << "\"/>\n";
  file << "<terraintypes>\n";

  for (auto terrain : terrains) {
    int id = tiles.compute_biome_id(terrain.first);
    file << "\t<terrain name=\"" << terrain.second << "\" tile=\"" << (id - FIRST_GID) << "\" />\n";
  }

  file << "</terraintypes>\n";

  for (auto value : tiles) {
    file << "<tile id=\"" << value.first << "\" terrain=";
    char sep = '"';
    for (auto where : { tile::detail::NW, tile::detail::NE, tile::detail::SW, tile::detail::SE }) {
      int biome = value.second.biome(where);
      int id = tiles.compute_biome_id(biome);
      file << sep << (id - FIRST_GID);
      sep = ',';
    }
    file << "\" />\n";
  }

  file << "</tileset>\n";

  // tile layer
  file << "<layer name=\"Ground Floor\" ";
  file << "width=\"" << tilemap.width() << "\" height=\"" << tilemap.height() << "\">\n";

  file << "<properties>\n";
  file << "\t<property name=\"kind\" value=\"ground\"/>\n";
  file << "</properties>\n";

  file << "<data encoding=\"csv\">\n";
  char sep = ' ';
  for (auto y : tilemap.y_range()) {
    for (auto x : tilemap.x_range()) {
      int gid = tiles.compute_id(tilemap(x, y));
      file << sep << gid;
      sep = ',';
    }
  }
  file << "</data>\n";

  file << "</layer>\n";

  // collision layer
  file << "<objectgroup color=\"#00c0c0\" name=\"No Trespassing Lines\" ";
  file << "width=\"" << tilemap.width() << "\" height=\"" << tilemap.height() << "\">\n";

  file << "<properties>\n";
  file << "\t<property name=\"floor\" value=\"0\"/>\n";
  file << "\t<property name=\"kind\" value=\"zone\"/>\n";
  file << "</properties>\n";

  auto hulls = mm::hull()(unit_map);

  for (auto hull : hulls) {
    auto first = hull.front();
    long x0 = first.x * TILE_SIZE - (TILE_SIZE / 2);
    long y0 = first.y * TILE_SIZE - (TILE_SIZE / 2);

    file << "<object name=\"Limit\" type=\"collision\" ";
    file << "x=\"" << x0 << "\" y=\"" << y0 << "\">\n";
    file << "<polygon points=";
    char sep = '"';
    for (auto point : hull) {
      long x = point.x * TILE_SIZE - (TILE_SIZE / 2);
      long y = point.y * TILE_SIZE - (TILE_SIZE / 2);

      file << sep << (x - x0) << ',' << (y - y0);
      sep = ' ';
    }
    file << "\"/>\n";
    file << "</object>\n";
  }

  file << "</objectgroup>\n";

  file << "</map>\n";


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
    generate_akagoria_map(node, "map.tmx", "biomes.pnm");

  } catch (std::exception& ex) {
    std::printf("Error: %s\n", ex.what());
    return 1;
  }

  return 0;
}
