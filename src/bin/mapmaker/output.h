#ifndef OUTPUT_H
#define OUTPUT_H

#include <iosfwd>

#include <mm/color_ramp.h>
#include <mm/heightmap.h>
#include <yaml-cpp/yaml.h>

namespace mm {

  void output_heightmap(const heightmap& map, YAML::Node node);
  void output_planemap(const planemap<bool>& map, const std::string& filename);

}

#endif // OUTPUT_H
