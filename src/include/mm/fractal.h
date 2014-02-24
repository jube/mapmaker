/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef MM_FRACTAL_H
#define MM_FRACTAL_H

#include <mm/heightmap.h>
#include <mm/random.h>

namespace mm {

  class fractal {
  public:
    typedef std::size_t size_type;

    fractal(std::function<double(double,double)> noise, double scale, size_type octaves = 8, double lacunarity = 2.0, double persistence = 0.5)
    : m_noise(noise)
    , m_scale(scale)
    , m_octaves(octaves)
    , m_lacunarity(lacunarity)
    , m_persistence(persistence)
    {
    }

    heightmap operator()(random_engine& engine, size_type width, size_type height) const;

  private:
    std::function<double(double,double)> m_noise;
    double m_scale;
    size_type m_octaves;
    double m_lacunarity;
    double m_persistence;
  };


}

#endif // MM_FRACTAL_H
