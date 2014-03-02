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
#include <fstream>
#include <iostream>

#include <mm/colorize.h>
#include <mm/shader.h>

#include "exception.h"
#include "print.h"

namespace mm {

  void output_heightmap(const heightmap& map, YAML::Node node) {
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

    std::ofstream file(filename);

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

      // see: http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
      mm::color_ramp ramp;
      ramp.add_color_stop(0.000, {  2,  43,  68}); // very dark blue: deep water
      ramp.add_color_stop(0.250, {  9,  62,  92}); // dark blue: water
      ramp.add_color_stop(0.490, { 17,  82, 112}); // blue: shallow water
      ramp.add_color_stop(0.500, { 69, 108, 118}); // light blue: shore
      ramp.add_color_stop(0.501, { 42, 102,  41}); // green: grass
      ramp.add_color_stop(0.750, {115, 128,  77}); // light green: veld
      ramp.add_color_stop(0.850, {153, 143,  92}); // brown: tundra
      ramp.add_color_stop(0.950, {179, 179, 179}); // grey: rocks
      ramp.add_color_stop(1.000, {255, 255, 255}); // white: snow

      auto colored = colorize(ramp, sea_level)(map);

      if (shaded) {
        colored = shader(sea_level)(colored, map);
      }

      colored.output_to_ppm(file);

    } else if (type == "grayscale") {
      map.output_to_pgm(file);
    } else {
      std::printf("Warning! Unknown output type: '%s'. No output generated.\n", type.c_str());
    }

  }

}
