#ifndef FINALIZERS_H
#define FINALIZERS_H

#include <functional>

#include <mm/heightmap.h>
#include <yaml-cpp/yaml.h>

namespace mm {

  typedef std::function<void(const heightmap&)> finalizer_function;

  finalizer_function get_finalizer(YAML::Node node, heightmap::size_type size);
  void finalize(const heightmap& map, finalizer_function finalizer, YAML::Node node);

}

#endif // FINALIZERS_H
