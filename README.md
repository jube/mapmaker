# MapMaker

MapMaker is a flexible [heightmap](http://en.wikipedia.org/wiki/Heightmap) generator that uses a wide range of well-known techniques:

* [value noise](http://en.wikipedia.org/wiki/Value_noise)
* [gradient noise](http://en.wikipedia.org/wiki/Gradient_noise), also known as [Perlin noise](http://en.wikipedia.org/wiki/Perlin_noise)
* [simplex noise](http://en.wikipedia.org/wiki/Simplex_noise)
* cell noise, also known as [Worley noise](http://en.wikipedia.org/wiki/Worley_noise)
* [diamond-square](http://en.wikipedia.org/wiki/Diamond-square_algorithm)
* many erosion algorithms (thermal, hydraulic, fast)

## Requirements

MapMaker is written in C++11 so you need a C++11 compiler like [Clang](http://clang.llvm.org/) or [GCC](http://gcc.gnu.org/). The `libmm0` library has no other dependency, the `mapmaker` binary depends on [yaml-cpp](https://code.google.com/p/yaml-cpp/).

## Build and install

You can download the sources directly from github:

    git clone https://github.com/jube/mapmaker.git

Then you have to use [CMake](http://www.cmake.org/) to build the project:

    cd mapmaker
    mkdir build
    cd build
    cmake ../src
    make

Finally, you can install the files (you may need root permissions):

    make install

## Use

MapMaker's library provides a [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/) file so you can use it to configure your project.

    pkg-config --cflags --libs mm0


## Authors

- Julien Bernard, julien dot bernard at univ dash fcomte dot fr

## ChangeLog

See [ChangeLog.md](https://github.com/jube/mapmaker/blob/master/ChangeLog.md).

## Copyright

This library is open source and is distributed under the [ISC licence](http://opensource.org/licenses/isc-license).
