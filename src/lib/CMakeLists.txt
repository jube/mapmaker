
set(LIBMM_SRC
  accessibility.cc
  cell_noise.cc
  color_ramp.cc
  cutoff.cc
  diamond_square.cc
  erosion_score.cc
  fast_erosion.cc
  fractal.cc
  gradient_noise.cc
  heightmap.cc
  hydraulic_erosion.cc
  islandize.cc
  logical_combine.cc
  normalize.cc
  ratio.cc
  reachability.cc
  sea_level.cc
  simplex_noise.cc
  slope.cc
  thermal_erosion.cc
  value_noise.cc
)

add_library(mm0 SHARED ${LIBMM_SRC})

install(
  TARGETS mm0
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
