#include <cassert>
#include <cstdio>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>

#include <mm/color_ramp.h>
#include <mm/curve.h>

#include <mm/diamond_square.h>
#include <mm/fractal.h>
#include <mm/gradient_noise.h>

#include <mm/thermal_erosion.h>
#include <mm/hydraulic_erosion.h>
#include <mm/fast_erosion.h>
#include <mm/erosion_score.h>

#include <mm/sea_level.h>

#include <mm/accessibility.h>
#include <mm/islandize.h>
#include <mm/normalize.h>
#include <mm/cutoff.h>
#include <mm/reachability.h>
#include <mm/slope.h>
#include <mm/logical_combine.h>

#include <yaml-cpp/yaml.h>

#include "exception.h"
#include "process.h"

static void usage() {
  std::printf("Usage: mapmaker <file>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage();
    return 0;
  }

  try {

    YAML::Node node = YAML::LoadFile(argv[1]);

    auto map = mm::process_generator(node);
    map = mm::process_modifiers(map, node);
    mm::process_finalizer(map, node);

  } catch (std::exception& ex) {
    std::printf("Error: %s\n", ex.what());
    return 1;
  }

#if 0

  //   const std::size_t size = 16384 + 1;
  const std::size_t size = 2048 + 1;
//   const std::size_t width = 3000;
//   const std::size_t height = 2000;


//   std::random_device dev;
//   auto seed = dev();
//   std::cerr << "seed: " << seed << '\n';
  mm::random_engine gen(0);

  auto start = std::chrono::steady_clock::now();
//   mm::diamond_square<double> generator;

  mm::fractal generator(mm::gradient_noise(gen, mm::curve_quintic<double>), 16, 2.0, 0.50);

  auto map = generator(gen, size, size);
  auto end = std::chrono::steady_clock::now();

  auto elapsed = end - start;
  std::cerr << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << '\n';

  map = mm::normalize()(map);

  std::ofstream file_00_1("map_00_1.pnm");
  output_pgm(file_00_1, map);

  map = mm::sea_level(0.4)(map);
  map = mm::islandize(0.15 * size)(map);
//   map = mm::islandize(0.25 * size)(map);

  std::cerr << "end normalize\n";

#if 0

  // http://libnoise.sourceforge.net/tutorials/tutorial3.html
  mm::color_ramp gradient_ln;
  gradient_ln.add_color_stop(0.000, {  0,   0, 128}); // deeps
  gradient_ln.add_color_stop(0.125, {  0,   0, 255}); // shallow
  gradient_ln.add_color_stop(0.500, {  0, 128, 255}); // shore
  gradient_ln.add_color_stop(0.531, {240, 240,  64}); // sand
  gradient_ln.add_color_stop(0.562, { 32, 160,   0}); // grass
  gradient_ln.add_color_stop(0.687, {224, 224,   0}); // dirt
  gradient_ln.add_color_stop(0.875, {128, 128, 128}); // rock
  gradient_ln.add_color_stop(1.000, {255, 255, 255}); // snow

  // https://code.google.com/p/fractalterraingeneration/source/browse/branches/simplex/main.cpp#25
  mm::color_ramp gradient_ft;
  gradient_ft.add_color_stop(0.000, {  0,   0,  55});
  gradient_ft.add_color_stop(0.500, {  0,  53, 106});
  gradient_ft.add_color_stop(0.501, {  0,  64,   0});
  gradient_ft.add_color_stop(0.850, {133, 182, 116});
  gradient_ft.add_color_stop(0.851, {167, 157, 147});
  gradient_ft.add_color_stop(1.000, {216, 223, 226});

  // http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
  mm::color_ramp gradient_bb;
  gradient_bb.add_color_stop(0.000, {  2,  43,  68}); // very dark blue: deep water
  gradient_bb.add_color_stop(0.250, {  9,  62,  92}); // dark blue: water
  gradient_bb.add_color_stop(0.490, { 17,  82, 112}); // blue: shallow water
  gradient_bb.add_color_stop(0.500, { 69, 108, 118}); // light blue: shore
  gradient_bb.add_color_stop(0.501, { 42, 102,  41}); // green: grass
  gradient_bb.add_color_stop(0.750, {115, 128,  77}); // light green: veld
  gradient_bb.add_color_stop(0.850, {153, 143,  92}); // brown: tundra
  gradient_bb.add_color_stop(0.950, {179, 179, 179}); // grey: rocks
  gradient_bb.add_color_stop(1.000, {255, 255, 255}); // white: snow

  std::ofstream file_ln("map_ln.pnm");
  output_ppm(file_ln, map, gradient_ln);

  std::ofstream file_ft("map_ft.pnm");
  output_ppm(file_ft, map, gradient_ft);

  std::ofstream file_bb("map_bb.pnm");
  output_ppm(file_bb, map, gradient_bb);
#endif

#if 1
  // https://code.google.com/p/fractalterraingeneration/source/browse/branches/simplex/main.cpp#25
  mm::color_ramp gradient_ft;
  gradient_ft.add_color_stop(0.000, {  0,   0,  55});
  gradient_ft.add_color_stop(0.500, {  0,  53, 106});
  gradient_ft.add_color_stop(0.501, {  0,  64,   0});
  gradient_ft.add_color_stop(0.850, {133, 182, 116});
  gradient_ft.add_color_stop(0.851, {167, 157, 147});
  gradient_ft.add_color_stop(1.000, {216, 223, 226});

  mm::color_ramp gradient_ll;
  gradient_ll.add_color_stop(0.000, {255, 255, 255});
  gradient_ll.add_color_stop(0.490, {255, 255, 255});
  gradient_ll.add_color_stop(0.500, {  0,   0, 128}); // blue
  gradient_ll.add_color_stop(0.510, {255, 255, 255});
  gradient_ll.add_color_stop(0.590, {255, 255, 255});
  gradient_ll.add_color_stop(0.600, {  0, 128,   0}); // green
  gradient_ll.add_color_stop(0.610, {255, 255, 255});
  gradient_ll.add_color_stop(0.690, {255, 255, 255});
  gradient_ll.add_color_stop(0.700, {115, 128,  77}); // light green
  gradient_ll.add_color_stop(0.710, {255, 255, 255});
  gradient_ll.add_color_stop(0.790, {255, 255, 255});
  gradient_ll.add_color_stop(0.800, {153, 143,  92}); // brown
  gradient_ll.add_color_stop(0.810, {255, 255, 255});
  gradient_ll.add_color_stop(0.890, {255, 255, 255});
  gradient_ll.add_color_stop(0.900, {128, 128, 128}); // grey
  gradient_ll.add_color_stop(0.910, {255, 255, 255});
  gradient_ll.add_color_stop(1.000, {255, 255, 255});

  std::ofstream file_ll("gradient_ll.pnm");
  output_gradient(file_ll, gradient_ll);


//   mm::thermal_erosion erosion(50, 4.0 / size, 0.4);
//   mm::hydraulic_erosion erosion(500, 0.01, 0.01, 0.5, 0.01);
//   mm::fast_erosion erosion1(100, 8.0 / size, 0.5);
  mm::fast_erosion erosion1(5, 8.0 / size, 0.5);
//   mm::thermal_erosion erosion2(2, 4.0 / size, 0.4);
  double es = 0.0;

  std::ofstream file_00("map_00.pnm");
  output_ppm(file_00, map, gradient_ft);
  es = mm::erosion_score()(map);
  std::cerr << "generated file00: " << es << '\n';

  start = std::chrono::steady_clock::now();
  map = erosion1(map);
//   map = erosion2(map);
  end = std::chrono::steady_clock::now();

  elapsed = end - start;
  std::cerr << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << '\n';

  std::ofstream file_01("map_01.pnm");
  output_ppm(file_01, map, gradient_ft);
  es = mm::erosion_score()(map);
  std::cerr << "generated file01: " << es << '\n';

  std::ofstream file_01_0("map_01_0.pnm");
  output_ppm(file_01_0, map, gradient_ll);

  auto sea_map = mm::cutoff(0.5)(map);
  std::ofstream file_01_1("map_01_1.pnm");
  output_pbm(file_01_1, sea_map);

  auto slope_map = mm::slope()(map);

  auto cutoff_map = mm::cutoff(8.0 / size)(slope_map);
  cutoff_map = mm::logical_combine()(cutoff_map, sea_map, [](bool lhs, bool rhs) { return lhs & !rhs; });

  std::ofstream file_01_2("map_01_2.pnm");
  output_pbm(file_01_2, cutoff_map);

  auto reachable_map = mm::reachability(3)(cutoff_map);

  std::ofstream file_01_3("map_01_3.pnm");
  output_pbm(file_01_3, reachable_map);

  auto accessible_map = mm::accessibility()(reachable_map);

  std::ofstream file_01_4("map_01_4.pnm");
  output_pbm(file_01_4, accessible_map);



//   erosion(map);
//
//   std::ofstream file_02("map_02.pnm");
//   output_ppm(file_02, map, gradient_ft);
//   std::cerr << "generated file02: " << mm::erosion_score(map) << '\n';
//
//   erosion(map);
//
//   std::ofstream file_03("map_03.pnm");
//   output_ppm(file_03, map, gradient_ft);
//   std::cerr << "generated file03: " << mm::erosion_score(map) << '\n';

#endif

#if 0
  // http://libnoise.sourceforge.net/tutorials/tutorial3.html
  mm::color_ramp gradient_ln;
  gradient_ln.add_color_stop(0.000, {  0,   0, 128}); // deeps
  gradient_ln.add_color_stop(0.125, {  0,   0, 255}); // shallow
  gradient_ln.add_color_stop(0.500, {  0, 128, 255}); // shore
  gradient_ln.add_color_stop(0.531, {240, 240,  64}); // sand
  gradient_ln.add_color_stop(0.562, { 32, 160,   0}); // grass
  gradient_ln.add_color_stop(0.687, {224, 224,   0}); // dirt
  gradient_ln.add_color_stop(0.875, {128, 128, 128}); // rock
  gradient_ln.add_color_stop(1.000, {255, 255, 255}); // snow

  // https://code.google.com/p/fractalterraingeneration/source/browse/branches/simplex/main.cpp#25
  mm::color_ramp gradient_ft;
  gradient_ft.add_color_stop(0.000, {  0,   0,  55});
  gradient_ft.add_color_stop(0.500, {  0,  53, 106});
  gradient_ft.add_color_stop(0.501, {  0,  64,   0});
  gradient_ft.add_color_stop(0.850, {133, 182, 116});
  gradient_ft.add_color_stop(0.851, {167, 157, 147});
  gradient_ft.add_color_stop(1.000, {216, 223, 226});

  // http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
  mm::color_ramp gradient_bb;
  gradient_bb.add_color_stop(0.000, {  2,  43,  68}); // very dark blue: deep water
  gradient_bb.add_color_stop(0.250, {  9,  62,  92}); // dark blue: water
  gradient_bb.add_color_stop(0.490, { 17,  82, 112}); // blue: shallow water
  gradient_bb.add_color_stop(0.500, { 69, 108, 118}); // light blue: shore
  gradient_bb.add_color_stop(0.501, { 42, 102,  41}); // green: grass
  gradient_bb.add_color_stop(0.750, {115, 128,  77}); // light green: veld
  gradient_bb.add_color_stop(0.850, {153, 143,  92}); // brown: tundra
  gradient_bb.add_color_stop(0.950, {179, 179, 179}); // grey: rocks
  gradient_bb.add_color_stop(1.000, {255, 255, 255}); // white: snow

  std::ofstream file_ln("gradient_ln.pnm");
  output_gradient(file_ln, gradient_ln);

  std::ofstream file_ft("gradient_ft.pnm");
  output_gradient(file_ft, gradient_ft);

  std::ofstream file_bb("gradient_bb.pnm");
  output_gradient(file_bb, gradient_bb);
#endif


#endif

  return 0;
}
