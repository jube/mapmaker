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
#include <stdexcept>

#include <yaml-cpp/yaml.h>

#include <mm/color.h>
#include <mm/heightmap.h>
#include <mm/random.h>

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


}



class bad_structure : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};


void generate_akagoria_map(YAML::Node node) {
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

  mm::heightmap map = mm::heightmap::input_from_pgm(heightmap_node.as<std::string>());


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
