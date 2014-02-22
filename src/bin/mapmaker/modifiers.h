#ifndef MODIFIERS_H
#define MODIFIERS_H

#include <functional>

#include <mm/heightmap.h>
#include <yaml-cpp/yaml.h>

namespace mm {

  typedef std::function<heightmap(const heightmap&)> modifier_function;

  modifier_function get_modifier(YAML::Node node, heightmap::size_type size);
  heightmap modify(const heightmap& map, modifier_function modifier, YAML::Node node);

}


#endif // MODIFIERS_H
