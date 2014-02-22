#include "process.h"

#include <cinttypes>

#include "exception.h"
#include "finalizers.h"
#include "generators.h"
#include "modifiers.h"
#include "output.h"

namespace mm {

  heightmap process_generator(YAML::Node node) {
    mm::random_engine::result_type seed = 0;

    auto seed_node = node["seed"];
    if (seed_node) {
      seed = seed_node.as<mm::random_engine::result_type>();
    } else {
      std::random_device dev;
      seed = dev();
      std::printf("Using 'random_device' for seed: %" PRIuFAST64 "\n", seed);
    }

    mm::random_engine engine(seed);

    auto generator_node = node["generator"];
    if (!generator_node) {
      throw mm::bad_structure("mapmaker: missing 'generator' definition");
    }

    auto generator = mm::get_generator(engine, generator_node);
    auto map = mm::generate(engine, generator, generator_node);

    return std::move(map);
  }

  heightmap process_modifiers(const heightmap& map, YAML::Node node) {
    heightmap current(map);

    auto size_max = std::max(map.width(), map.height());
    auto size_min = std::min(map.width(), map.height());
    auto size = size_min + (size_max - size_min) / 2; // to avoid overflow

    auto modifiers_node = node["modifiers"];

    if (modifiers_node) {
      if (!modifiers_node.IsSequence()) {
        throw mm::bad_structure("mapmaker: wrong type for 'modifiers'");
      }

      for (auto modifier_node : modifiers_node) {
        auto modifier = mm::get_modifier(modifier_node, size);
        current = mm::modify(current, modifier, modifier_node);
      }
    }

    return std::move(current);
  }

  void process_finalizer(const heightmap& map, YAML::Node node) {
    auto size_max = std::max(map.width(), map.height());
    auto size_min = std::min(map.width(), map.height());
    auto size = size_min + (size_max - size_min) / 2; // to avoid overflow

    auto finalizer_node = node["finalizer"];

    if (finalizer_node) {
      auto finalizer = mm::get_finalizer(finalizer_node, size);
      mm::finalize(map, finalizer, finalizer_node);
    }
  }

}
