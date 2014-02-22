#ifndef GENERATORS_H
#define GENERATORS_H

#include <functional>

#include <mm/random.h>
#include <mm/heightmap.h>
#include <yaml-cpp/yaml.h>

namespace mm {

  typedef std::function<heightmap(random_engine&, position::size_type, position::size_type)> generator_function;

  generator_function get_generator(random_engine& engine, YAML::Node node);
  heightmap generate(random_engine& engine, generator_function generator, YAML::Node node);

}

#endif // GENERATORS_H
