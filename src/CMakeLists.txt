cmake_minimum_required(VERSION 2.8)

project(MM CXX)

include(CPackConfig.cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(YAMLCPP yaml-cpp)

add_definitions(-Wall -g -O2)

if (WIN32)
  add_definitions(-std=c++0x)
else (WIN32)
  add_definitions(-std=c++11)
endif (WIN32)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib)
add_subdirectory(bin)

configure_file("${CMAKE_SOURCE_DIR}/libmm0.pc.in" "${CMAKE_BINARY_DIR}/libmm0.pc" @ONLY)
install(
  FILES "${CMAKE_BINARY_DIR}/libmm0.pc"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig"
)


install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/mm"
  DESTINATION include
)
