include_directories(${YAMLCPP_INCLUDE_DIRS})
link_directories(${YAMLCPP_LIBRARY_DIRS})

set(BIOME_MAP_SRC
  biome-map.cc
)

add_executable(biome-map ${BIOME_MAP_SRC})
target_link_libraries(biome-map mm0 ${YAMLCPP_LIBRARIES})

install(
  TARGETS biome-map
  RUNTIME DESTINATION bin
)
