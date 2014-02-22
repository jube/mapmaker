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

#include "exception.h"
#include "print.h"

namespace mm {

  static void output_pbm(std::ostream& o, const mm::planemap<bool>& map) {
    o << "P1\n";
    o << map.width() << ' ' << map.height() << '\n';

    for (std::size_t y = 0; y < map.height(); ++y) {
      for (std::size_t x = 0; x < map.width(); ++x) {
        o << (map(x, y) ? '0' : '1') << ' ';
      }

      o << '\n';
    }
  }

  #define WHITE 65535

  static void output_pgm(std::ostream& o, const mm::heightmap& map) {
    o << "P2\n";
    o << map.width() << ' ' << map.height() << '\n';
    o << WHITE << '\n';

    for (std::size_t y = 0; y < map.height(); ++y) {
      for (std::size_t x = 0; x < map.width(); ++x) {
        unsigned val = static_cast<unsigned>(map(x, y) * WHITE);
        assert(0 <= val && val <= WHITE);
        o << val << ' ';
      }

      o << '\n';
    }
  }

  static void output_ppm(std::ostream& o, const mm::heightmap& map, const mm::color_ramp& ramp) {
    o << "P3\n";
    o << map.width() << ' ' << map.height() << '\n';
    o << static_cast<unsigned>(mm::color::max) << '\n';

    for (std::size_t y = 0; y < map.height(); ++y) {
      for (std::size_t x = 0; x < map.width(); ++x) {
        auto color = ramp.get_color(map(x, y));
        o << static_cast<unsigned>(color.red_channel()) << ' '
            << static_cast<unsigned>(color.green_channel()) << ' '
            << static_cast<unsigned>(color.blue_channel()) << ' ';
      }

      o << '\n';
    }
  }

  static void output_color_ramp(std::ostream& o, const mm::color_ramp& ramp) {
    const std::size_t width = 512;
    const std::size_t height = 32;
    mm::heightmap map(width, height);

    for (std::size_t x = 0; x < map.width(); ++x) {
      double value = static_cast<double>(x) / width;
      for (std::size_t y = 0; y < map.height(); ++y) {
        map(x, y) = value;
      }
    }

    output_ppm(o, map, ramp);
  }

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

      output_ppm(file, map, ramp);
    } else if (type == "grayscale") {
      output_pgm(file, map);
    } else {
      std::printf("Warning! Unknown output type: '%s'. No output generated.\n", type.c_str());
    }

  }

  void output_planemap(const planemap<bool>& map, const std::string& filename) {
    std::ofstream file(filename);
    output_pbm(file, map);
  }

}
