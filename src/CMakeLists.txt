cmake_minimum_required(VERSION 3.10)

project(MM
  VERSION 1.0.0
  LANGUAGES CXX
)

include(CPackConfig.cmake)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)

pkg_check_modules(YAMLCPP yaml-cpp)

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4 /utf-8")
  link_directories(${Boost_LIBRARY_DIRS})
  # for more information on the last two switches:
  # - https://blogs.msdn.microsoft.com/vcblog/2016/06/07/standards-version-switches-in-the-compiler/
  # - https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG           "-O1 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
endif()

include_directories("${CMAKE_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})

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
