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
#include "output.h"

#include <cassert>

#include <mm/colorize.h>
#include <mm/playability.h>
#include <mm/reachability.h>
#include <mm/shader.h>

#include "exception.h"
#include "print.h"

namespace mm {

  void output_heightmap(const heightmap& map, YAML::Node node, random_engine& engine) {
    auto filename_node = node["filename"];
    if (!filename_node) {
      throw bad_structure("mapmaker: missing 'filename' in output definition");
    }
    auto filename = filename_node.as<std::string>();

    auto type_node = node["type"];
    if (!type_node) {
      throw bad_structure("mapmaker: missing 'type' in output definition");
    }
    auto type = type_node.as<std::string>();

    print_indent();
    std::printf("\toutput: '" BEGIN_FILE "%s" END_FILE "'\n", filename.c_str());

    if (type == "colored") {
      auto parameters_node = node["parameters"];
      if (!parameters_node) {
        throw bad_structure("mapmaker: missing 'parameters' in 'colored' output definition");
      }

      auto sea_level_node = parameters_node["sea_level"];
      if (!sea_level_node) {
        throw bad_structure("mapmaker: missing 'sea_level' in 'colored' output parameters");
      }
      auto sea_level = sea_level_node.as<double>();

      auto shaded_node = parameters_node["shaded"];
      if (!shaded_node) {
        throw bad_structure("mapmaker: missing 'shaded' in 'colored' output parameters");
      }
      auto shaded = shaded_node.as<bool>();

      color_ramp ramp = color_ramp::basic();
      auto colored = colorize(ramp, sea_level)(map);

      if (shaded) {
        colored = shader(sea_level)(colored, map);
      }

      colored.output_to_ppm(filename);

#if 0
    } else if (type == "tiled") {
      auto parameters_node = node["parameters"];
      if (!parameters_node) {
        throw bad_structure("mapmaker: missing 'parameters' in 'tiled' output definition");
      }

      auto sea_level_node = parameters_node["sea_level"];
      if (!sea_level_node) {
        throw bad_structure("mapmaker: missing 'sea_level' in 'tiled' output parameters");
      }
      auto sea_level = sea_level_node.as<double>();

      auto nu_node = parameters_node["unit_size"];
      if (!nu_node) {
        throw bad_structure("mapmaker: missing 'unit_size' in 'tiled' output parameters");
      }
      auto nu = nu_node.as<reachability::size_type>();

      auto tu_node = parameters_node["unit_talus"];
      if (!tu_node) {
        throw bad_structure("mapmaker: missing 'unit_talus' in 'tiled' output parameters");
      }
      auto tu = tu_node.as<double>();

      auto rivers_node = parameters_node["rivers"];
      if (!rivers_node) {
        throw bad_structure("mapmaker: missing 'rivers' in 'tiled' output parameters");
      }
      auto rivers = rivers_node.as<unsigned>();

      auto min_source_altitude_node = parameters_node["min_source_altitude"];
      if (!min_source_altitude_node) {
        throw bad_structure("mapmaker: missing 'min_source_altitude' in 'tiled' output parameters");
      }
      auto min_source_altitude = min_source_altitude_node.as<double>();

//       biomeset set = color_ramp::basic().compute_biomeset(sea_level);
      biomeset set = biomeset::whittaker();

      set.output_to_ppm("biomeset.ppm");

      auto tiled = decorate(sea_level, rivers, min_source_altitude)(map, set, engine);
      auto colored = biomize(biomize::kind::DETAILED)(tiled, set);

      auto size_max = std::max(map.width(), map.height());
      auto size_min = std::min(map.width(), map.height());
      auto size = size_min + (size_max - size_min) / 2; // to avoid overflow

      binarymap unit_map;
      std::tie(std::ignore, unit_map, std::ignore) = playability(sea_level, nu, nu, tu / size, tu / size, false)(map);
      tilize()(tiled, set, unit_map, "map.tmx", "biome.pnm");

//       colored = shader(sea_level)(colored, map);

      colored.output_to_ppm(filename);
#endif

    } else if (type == "grayscale") {
      map.output_to_pgm(filename);
    } else {
      std::printf("Warning! Unknown output type: '%s'. No output generated.\n", type.c_str());
    }

  }

}
