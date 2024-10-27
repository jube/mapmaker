
set(LIBMM_SRC
  accessibility.cc
  binarymap.cc
  cell_noise.cc
  colorize.cc
  colormap.cc
  color_ramp.cc
  cutoff.cc
  diamond_square.cc
  erosion_score.cc
  fast_erosion.cc
  flatten.cc
  fractal.cc
  gaussize.cc
  gradient_noise.cc
  heightmap.cc
  hills.cc
  hull.cc
  hydraulic_erosion.cc
  invert.cc
  islandize.cc
  logical_combine.cc
  midpoint_displacement.cc
  normalize.cc
  playability.cc
  ratio.cc
  reachability.cc
  shader.cc
  simplex_noise.cc
  slope.cc
  smooth.cc
  thermal_erosion.cc
  value_noise.cc
)

add_library(mm0 SHARED ${LIBMM_SRC})

target_compile_features(mm0
  PUBLIC
    cxx_std_17
)

set_target_properties(mm0
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS mm0
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
