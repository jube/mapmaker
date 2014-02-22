#ifndef MM_FRACTAL_H
#define MM_FRACTAL_H

#include <iostream>
#include <cassert>
#include <cmath>

#include <mm/heightmap.h>
#include <mm/random.h>

namespace mm {

  class fractal {
  public:
    typedef std::size_t size_type;

    fractal(std::function<double(double,double)> noise, size_type octaves = 8, double lacunarity = 2.0, double persistence = 0.5)
    : m_noise(noise)
    , m_octaves(octaves)
    , m_lacunarity(lacunarity)
    , m_persistence(persistence)
    {
    }

    heightmap operator()(random_engine& engine, size_type width, size_type height) const;

  private:
    std::function<double(double,double)> m_noise;
    size_type m_octaves;
    double m_lacunarity;
    double m_persistence;
  };


}

#endif // MM_FRACTAL_H
