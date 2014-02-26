
set(LIBMM_SRC
  accessibility.cc
  cell_noise.cc
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
  hydraulic_erosion.cc
  islandize.cc
  logical_combine.cc
  midpoint_displacement.cc
  normalize.cc
  ratio.cc
  reachability.cc
  simplex_noise.cc
  slope.cc
  smooth.cc
  thermal_erosion.cc
  value_noise.cc
)

add_library(mm0 SHARED ${LIBMM_SRC})

install(
  TARGETS mm0
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
