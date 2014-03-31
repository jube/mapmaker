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
#include <mm/biomeset.h>

#include <cassert>

#include <mm/biomize.h>
#include <mm/tilemap.h>

namespace mm {

  bool biomeset::add_terrain(const biome& biome) {
    auto ret = m_terrains.emplace(m_terrain_id++, biome);
    return ret.second;
  }

  int biomeset::compute_biome(double altitude, double humidity, bool water) const {
    for (auto value : m_terrains) {
      if (value.second.match(altitude, humidity, water)) {
        return value.first;
      }
    }

    return -1;
  }

  bool biomeset::has_higher_priority(int biome_id, int other_biome_id) const {
    auto it = m_terrains.find(biome_id);
    assert(it != m_terrains.end());

    auto it_other = m_terrains.find(other_biome_id);
    assert(it_other != m_terrains.end());

    auto biome = it->second;
    auto other_biome = it_other->second;

    return biome.has_higher_priority(other_biome);
  }

  color biomeset::biome_representation(int biome) const {
    auto ret = m_terrains.find(biome);

    if (ret == m_terrains.end()) {
      return color::black();
    }

    return ret->second.representation();
  }

#define TILESET_SIZE 256

  void biomeset::output_to_ppm(std::ostream& file) const {
    tilemap map(2 * TILESET_SIZE, TILESET_SIZE);

    for (tilemap::size_type x = 0; x < TILESET_SIZE; ++x) {
      for (tilemap::size_type y = 0; y < TILESET_SIZE; ++y) {
        double altitude = 1.0 - static_cast<double>(y) / TILESET_SIZE;
        double humidity = static_cast<double>(x) / TILESET_SIZE;

        map(x, y).set_biome(this->compute_biome(altitude, humidity, false));
        map(x + TILESET_SIZE, y).set_biome(this->compute_biome(altitude, humidity, true));
      }
    }

    auto colored = biomize(biomize::kind::SIMPLE)(map, *this);
    colored.output_to_ppm(file);
  }


  biomeset biomeset::whittaker() {
    biomeset set;
    /*
     * water
     */
    set.add_terrain({ "Ocean",                      { 0x44, 0x44, 0x7A }, { 0, 0.5 }, { 0, 1 }, true });
    set.add_terrain({ "Marsh",                      { 0x2F, 0x66, 0x66 }, { 0.5, 0.55 }, { 0, 1 }, true });
    set.add_terrain({ "Lake",                       { 0x33, 0x66, 0x99 }, { 0.55, 0.9 }, { 0, 1 }, true });
    set.add_terrain({ "Ice",                        { 0x99, 0xFF, 0xFF }, { 0.9, 1 }, { 0, 1 }, true });
    /*
     * non-water
     */
    set.add_terrain({ "Beach",                      { 0xA0, 0x90, 0x77 }, { 0.5, 0.52 }, { 0, 1 }, false });
    // low altitudes
    set.add_terrain({ "Subtropical Desert",         { 0xD2, 0xB9, 0x8B }, { 0.52, 0.65 }, { 0, 0.16 }, false });
    set.add_terrain({ "Grassland",                  { 0x88, 0xAA, 0x55 }, { 0.52, 0.65 }, { 0.16, 0.33 }, false });
    set.add_terrain({ "Tropical Seasonal Forest",   { 0x55, 0x99, 0x44 }, { 0.52, 0.65 }, { 0.33, 0.66 }, false });
    set.add_terrain({ "Tropical Rain Forest",       { 0x33, 0x77, 0x55 }, { 0.52, 0.65 }, { 0.66, 1 }, false });
    // mid altitudes
    set.add_terrain({ "Temperate Desert",           { 0xC9, 0xD2, 0x9B }, { 0.65, 0.8 }, { 0, 0.16 }, false });
    set.add_terrain({ "Grassland",                  { 0x88, 0xAA, 0x55 }, { 0.65, 0.8 }, { 0.16, 0.5 }, false });
    set.add_terrain({ "Temperate Deciduous Forest", { 0x67, 0x94, 0x59 }, { 0.65, 0.8 }, { 0.5, 0.83 }, false });
    set.add_terrain({ "Temperate Rain Forest",      { 0x44, 0x88, 0x55 }, { 0.65, 0.8 }, { 0.83, 1 }, false });
    // high altitudes
    set.add_terrain({ "Temperate Desert",           { 0xC9, 0xD2, 0x9B }, { 0.8, 0.9 }, { 0, 0.33 }, false });
    set.add_terrain({ "Shrubland",                  { 0x88, 0x99, 0x77 }, { 0.8, 0.9 }, { 0.33, 0.66 }, false });
    set.add_terrain({ "Taiga",                      { 0x99, 0xAA, 0x77 }, { 0.8, 0.9 }, { 0.66, 1 }, false });
    // very high altitudes
    set.add_terrain({ "Scorched",                   { 0x55, 0x55, 0x55 }, { 0.9, 1 }, { 0, 0.16 }, false });
    set.add_terrain({ "Bare",                       { 0x88, 0x88, 0x88 }, { 0.9, 1 }, { 0.16, 0.33 }, false });
    set.add_terrain({ "Tundra",                     { 0xBB, 0xBB, 0xAA }, { 0.9, 1 }, { 0.33, 0.5 }, false });
    set.add_terrain({ "Snow",                       { 0xFF, 0xFF, 0xFF }, { 0.9, 1 }, { 0.5, 1 }, false });

    return set;
  }

}
