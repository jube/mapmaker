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
#include "generators.h"

#include <cassert>
#include <chrono>

#include <mm/cell_noise.h>
#include <mm/curve.h>
#include <mm/diamond_square.h>
#include <mm/distance.h>
#include <mm/fractal.h>
#include <mm/gradient_noise.h>
#include <mm/normalize.h>
#include <mm/simplex_noise.h>
#include <mm/value_noise.h>

#include "exception.h"
#include "output.h"
#include "print.h"

namespace mm {

  /*
   * Noise
   */
  typedef std::function<double(double)> curve_function;

  static curve_function get_curve(const std::string& name) {
    if (name == "linear") {
      return curve_linear<double>;
    }

    if (name == "cubic") {
      return curve_cubic<double>;
    }

    if (name == "quintic") {
      return curve_quintic<double>;
    }

    if (name == "cosine") {
      return curve_cosine<double>;
    }

    std::printf("Warning! Unknown curve: '%s'. Using linear curve.\n", name.c_str());
    return curve_linear<double>;
  }

  typedef std::function<double(double,double)> noise_function;

  static double null_noise(double x, double y) {
    return 0.0;
  }

  static noise_function get_gradient_noise(random_engine& engine, YAML::Node node) {
    auto curve_node = node["curve"];

    if (!curve_node) {
      throw bad_structure("mapmaker: missing 'curve' in 'gradient' noise definition");
    }

    auto curve_name = curve_node.as<std::string>();
    auto curve = get_curve(curve_name);

    return gradient_noise(engine, curve);
  }

  static noise_function get_value_noise(random_engine& engine, YAML::Node node) {
    auto curve_node = node["curve"];

    if (!curve_node) {
      throw bad_structure("mapmaker: missing 'curve' in 'value' noise definition");
    }

    auto curve_name = curve_node.as<std::string>();
    auto curve = get_curve(curve_name);

    return value_noise(engine, curve);
  }

  typedef std::function<double(const vector2&, const vector2&)> distance_function;

  static distance_function get_distance(const std::string& name) {
    if (name == "manhattan") {
      return distance_manhattan;
    }

    if (name == "euclidean") {
      return distance_euclidean;
    }

    if (name == "chebyshev") {
      return distance_chebyshev;
    }

    std::printf("Warning! Unknown distance: '%s'. Using euclidean curve.\n", name.c_str());
    return distance_euclidean;
  }

  static noise_function get_cell_noise(random_engine& engine, YAML::Node node) {
    auto count_node = node["count"];
    if (!count_node) {
      throw bad_structure("mapmaker: missing 'count' in 'cell' noise definition");
    }
    auto count = count_node.as<cell_noise::size_type>();

    auto distance_node = node["distance"];

    if (!distance_node) {
      throw bad_structure("mapmaker: missing 'distance' in 'cell' noise definition");
    }

    auto distance_name = distance_node.as<std::string>();
    auto distance = get_distance(distance_name);

    auto coeffs_node = node["coeffs"];

    if (!coeffs_node) {
      throw bad_structure("mapmaker: missing 'coeffs' in 'cell' noise definition");
    }

    assert(coeffs_node.IsSequence()); // TODO: warning

    std::vector<double> coeffs;
    for (std::size_t i = 0; i < coeffs_node.size(); ++i) {
      coeffs.push_back(coeffs_node[i].as<double>()); // TODO: verify that it is a scalar
    }

    return cell_noise(engine, count, distance, std::move(coeffs));
  }


  static noise_function get_simplex_noise(random_engine& engine, YAML::Node node) {
    return simplex_noise(engine);
  }
  /*
   * Generators
   */
  static heightmap null_generator(random_engine&, position::size_type width, position::size_type height) {
    heightmap map(width, height);
    return std::move(map);
  }


  static generator_function get_fractal_generator(random_engine& engine, YAML::Node node) {
    auto noise_node = node["noise"];
    if (!noise_node) {
      throw bad_structure("mapmaker: missing 'noise' in 'fractal' generator parameters");
    }

    auto noise_parameters_node = node["noise_parameters"];
//     if (!noise_parameters_node) {
//       throw bad_structure("mapmaker: missing 'noise_parameters' in 'fractal' generator parameters");
//     }

    noise_function noise;
    auto noise_name = noise_node.as<std::string>();

    if (noise_name == "gradient") {
      noise = get_gradient_noise(engine, noise_parameters_node);
    } else if (noise_name == "cell") {
      noise = get_cell_noise(engine, noise_parameters_node);
    } else if (noise_name == "value") {
      noise = get_value_noise(engine, noise_parameters_node);
    } else if (noise_name == "simplex") {
      noise = get_simplex_noise(engine, noise_parameters_node);
    } else {
      std::printf("Warning! Unknown noise: '%s'. Using null noise.\n", noise_name.c_str());
      noise = null_noise;
    }

    auto octaves_node = node["octaves"];
    if (!octaves_node) {
      throw bad_structure("mapmaker: missing 'octaves' in 'fractal' generator parameters");
    }
    auto octaves = octaves_node.as<fractal::size_type>();

    auto lacunarity_node = node["lacunarity"];
    if (!lacunarity_node) {
      throw bad_structure("mapmaker: missing 'lacunarity' in 'fractal' generator parameters");
    }
    auto lacunarity = lacunarity_node.as<double>();

    auto persistence_node = node["persistence"];
    if (!persistence_node) {
      throw bad_structure("mapmaker: missing 'persistence' in 'fractal' generator parameters");
    }
    auto persistence = persistence_node.as<double>();

    return fractal(noise, octaves, lacunarity, persistence);
  }


  static generator_function get_diamond_square_generator(random_engine& engine, YAML::Node node) {
    auto values_node = node["values"];
    if (!values_node) {
      throw bad_structure("mapmaker: missing 'values' in 'diamond-square' generator parameters");
    }

    if (values_node.IsSequence()) {
      if (values_node.size() != 4) {
        throw bad_structure("mapmaker: wrong array size in 'values' in 'diamond-square' generator parameters");
      }

      std::array<double, 4> values;
      for (std::size_t i = 0; i < values_node.size(); ++i) {
        values.at(i) = values_node[i].as<double>(); // TODO: verify that it is a scalar
      }

      return diamond_square(values[0], values[1], values[2], values[3]);
    }

    assert(values_node.IsScalar());

    double value = values_node.as<double>();
    return diamond_square(value);
  }
  /*
   * API
   */

  generator_function get_generator(random_engine& engine, YAML::Node node) {
    auto name_node = node["name"];
    if (!name_node) {
      throw bad_structure("mapmaker: missing 'name' in generator definition");
    }
    auto name = name_node.as<std::string>();

    std::printf(BEGIN_TYPE "generator" END_TYPE ": '" BEGIN_NAME "%s" END_NAME "'\n", name.c_str());

    auto parameters_node = node["parameters"];

    if (name == "fractal") {
      return get_fractal_generator(engine, parameters_node);
    }

    if (name == "diamond-square") {
      return get_diamond_square_generator(engine, parameters_node);
    }

    return null_generator;
  }

  heightmap generate(random_engine& engine, generator_function generator, YAML::Node node) {
    auto size_node = node["size"];
    if (!size_node) {
      throw bad_structure("mapmaker: missing 'size' in generator definition");
    }

    auto width_node = size_node["width"];
    if (!width_node) {
      throw bad_structure("mapmaker: missing 'width' in generator definition");
    }
    auto width = width_node.as<position::size_type>();

    auto height_node = size_node["height"];
    if (!height_node) {
      throw bad_structure("mapmaker: missing 'height' in generator definition");
    }
    auto height = height_node.as<position::size_type>();

    std::printf("\tsize: %zu x %zu\n", width, height);

    auto start = std::chrono::steady_clock::now();
    auto map = generator(engine, width, height);
    map = normalize()(map);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    std::printf("\tduration: %lu ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

    auto output_node = node["output"];

    if (output_node) {
      output_heightmap(map, output_node);
    }

    return std::move(map);
  }

}
