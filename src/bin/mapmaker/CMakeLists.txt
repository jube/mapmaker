include_directories(${YAMLCPP_INCLUDE_DIRS})
link_directories(${YAMLCPP_LIBRARY_DIRS})

set(MAPMAKER_SRC
  finalizers.cc
  generators.cc
  mapmaker.cc
  modifiers.cc
  output.cc
  print.cc
  process.cc
)

add_executable(mapmaker ${MAPMAKER_SRC})
target_link_libraries(mapmaker mm0 ${YAMLCPP_LIBRARIES})

install(
  TARGETS mapmaker
  RUNTIME DESTINATION bin
)
