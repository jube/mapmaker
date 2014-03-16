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
#include "modifiers.h"

#include <cinttypes>
#include <chrono>

#include <mm/fast_erosion.h>
#include <mm/flatten.h>
#include <mm/gaussize.h>
#include <mm/hydraulic_erosion.h>
#include <mm/islandize.h>
#include <mm/normalize.h>
#include <mm/smooth.h>
#include <mm/thermal_erosion.h>

#include "exception.h"
#include "output.h"
#include "process.h"
#include "print.h"

namespace mm {

  namespace {

    class intercept {
    public:
      intercept(YAML::Node node)
      : m_node(node) {
      }

      heightmap operator()(const heightmap& src) {
        increment_indent();
        auto map = process_modifiers(src, m_node);
        process_finalizer(map, m_node);
        decrement_indent();

        return src;
      }

    private:
      YAML::Node m_node;
    };

  }

  /*
   * Modifiers
   */

  static heightmap null_modifier(const heightmap& map) {
    return map;
  }

  static modifier_function get_intercept_modifier(YAML::Node node, heightmap::size_type size) {
    return intercept(node);
  }

  static modifier_function get_islandize_modifier(YAML::Node node, heightmap::size_type size) {
    auto border_node = node["border"];
    if (!border_node) {
      throw bad_structure("mapmaker: missing 'border' in 'islandize' modifier parameters");
    }
    auto border = border_node.as<double>();

    return islandize(border * size);
  }

  static modifier_function get_gaussize_modifier(YAML::Node node, heightmap::size_type size) {
    auto spread_node = node["spread"];
    if (!spread_node) {
      throw bad_structure("mapmaker: missing 'spread' in 'gaussize' modifier parameters");
    }
    auto spread = spread_node.as<double>();

    return gaussize(spread * size);
  }

  static modifier_function get_thermal_erosion_modifier(YAML::Node node, heightmap::size_type size) {
    auto iterations_node = node["iterations"];
    if (!iterations_node) {
      throw bad_structure("mapmaker: missing 'iterations' in 'thermal-erosion' modifier parameters");
    }
    auto iterations = iterations_node.as<fast_erosion::size_type>();

    auto talus_node = node["talus"];
    if (!talus_node) {
      throw bad_structure("mapmaker: missing 'talus' in 'thermal-erosion' modifier parameters");
    }
    auto talus = talus_node.as<double>();

    auto fraction_node = node["fraction"];
    if (!fraction_node) {
      throw bad_structure("mapmaker: missing 'fraction' in 'thermal-erosion' modifier parameters");
    }
    auto fraction = fraction_node.as<double>();

    return thermal_erosion(iterations, talus / size, fraction);
  }

  static modifier_function get_fast_erosion_modifier(YAML::Node node, heightmap::size_type size) {
    auto iterations_node = node["iterations"];
    if (!iterations_node) {
      throw bad_structure("mapmaker: missing 'iterations' in 'fast-erosion' modifier parameters");
    }
    auto iterations = iterations_node.as<fast_erosion::size_type>();

    auto talus_node = node["talus"];
    if (!talus_node) {
      throw bad_structure("mapmaker: missing 'talus' in 'fast-erosion' modifier parameters");
    }
    auto talus = talus_node.as<double>();

    auto fraction_node = node["fraction"];
    if (!fraction_node) {
      throw bad_structure("mapmaker: missing 'fraction' in 'fast-erosion' modifier parameters");
    }
    auto fraction = fraction_node.as<double>();

    return fast_erosion(iterations, talus / size, fraction);
  }

  static modifier_function get_hydraulic_erosion_modifier(YAML::Node node, heightmap::size_type size) {
    auto iterations_node = node["iterations"];
    if (!iterations_node) {
      throw bad_structure("mapmaker: missing 'iterations' in 'fast-erosion' modifier parameters");
    }
    auto iterations = iterations_node.as<fast_erosion::size_type>();

    auto rain_node = node["rain_amount"];
    if (!rain_node) {
      throw bad_structure("mapmaker: missing 'rain_amount' in 'hydraulic-erosion' modifier parameters");
    }
    auto rain = rain_node.as<double>();

    auto solubility_node = node["solubility"];
    if (!solubility_node) {
      throw bad_structure("mapmaker: missing 'solubility' in 'hydraulic-erosion' modifier parameters");
    }
    auto solubility = solubility_node.as<double>();

    auto evaporation_node = node["evaporation"];
    if (!evaporation_node) {
      throw bad_structure("mapmaker: missing 'evaporation' in 'hydraulic-erosion' modifier parameters");
    }
    auto evaporation = evaporation_node.as<double>();

    auto capacity_node = node["capacity"];
    if (!capacity_node) {
      throw bad_structure("mapmaker: missing 'capacity' in 'hydraulic-erosion' modifier parameters");
    }
    auto capacity = capacity_node.as<double>();

    return hydraulic_erosion(iterations, rain, solubility, evaporation, capacity);
  }

  static modifier_function get_flatten_modifier(YAML::Node node, heightmap::size_type size) {
    auto factor_node = node["factor"];
    if (!factor_node) {
      throw bad_structure("mapmaker: missing 'factor' in 'flatten' modifier parameters");
    }
    auto factor = factor_node.as<double>();

    return flatten(factor);
  }

  static modifier_function get_smooth_modifier(YAML::Node node, heightmap::size_type size) {
    auto iterations_node = node["iterations"];
    if (!iterations_node) {
      throw bad_structure("mapmaker: missing 'iterations' in 'smooth' modifier parameters");
    }
    auto iterations = iterations_node.as<smooth::size_type>();

    return smooth(iterations);
  }

  /*
   * API
   */

  modifier_function get_modifier(YAML::Node node, heightmap::size_type size) {
    auto name_node = node["name"];
    if (!name_node) {
      throw bad_structure("mapmaker: missing 'name' in modifier definition");
    }
    auto name = name_node.as<std::string>();

    print_indent();
    std::printf(BEGIN_TYPE "modifier" END_TYPE ": '" BEGIN_NAME "%s" END_NAME "'\n", name.c_str());

    auto parameters_node = node["parameters"];

    if (name == "intercept") {
      return get_intercept_modifier(parameters_node, size);
    }

    if (name == "islandize") {
      return get_islandize_modifier(parameters_node, size);
    }

    if (name == "gaussize") {
      return get_gaussize_modifier(parameters_node, size);
    }

    if (name == "fast-erosion") {
      return get_fast_erosion_modifier(parameters_node, size);
    }

    if (name == "hydraulic-erosion") {
      return get_hydraulic_erosion_modifier(parameters_node, size);
    }

    if (name == "thermal-erosion") {
      return get_thermal_erosion_modifier(parameters_node, size);
    }

    if (name == "flatten") {
      return get_flatten_modifier(parameters_node, size);
    }

    if (name == "smooth") {
      return get_smooth_modifier(parameters_node, size);
    }

    return null_modifier;
  }

  heightmap modify(const heightmap& src, modifier_function modifier, YAML::Node node) {
    auto start = std::chrono::steady_clock::now();
    auto map = modifier(src);
    map = normalize()(map);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;

    print_indent();
    std::printf("\tduration: %" PRId64 " ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

    auto output_node = node["output"];

    if (output_node) {
      output_heightmap(map, output_node);
    }

    return map;
  }

}
