#ifndef PROCESS_H
#define PROCESS_H

#include <mm/heightmap.h>
#include <yaml-cpp/yaml.h>

namespace mm {

  heightmap process_generator(YAML::Node node);
  heightmap process_modifiers(const heightmap& map, YAML::Node node);
  void process_finalizer(const heightmap& map, YAML::Node node);

}

#endif // PROCESS_H
