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
#include <mm/simplex_noise.h>

#include <cassert>
#include <cmath>

#include <mm/curve.h>

namespace mm {

  simplex_noise::simplex_noise(random_engine& engine)
  {
    // initialize permutation
    for (uint8_t i = 0; i < 255; ++i) {
      m_perm[i] = i;
    }

    m_perm[255] = 255; // to avoid an infinite loop in the previous for loop

    // generate permutation
    std::uniform_int_distribution<uint8_t> dist_perm(0, 255);
    for (unsigned i = 0; i < 2560; ++i) {
      uint8_t j = dist_perm(engine);
      uint8_t k = dist_perm(engine);
      std::swap(m_perm[j], m_perm[k]);
    }

  }

  static const vector2 s_gradients[8] = {
    { -1.0, -1.0 },
    {  1.0,  0.0 },
    { -1.0,  0.0 },
    {  1.0,  1.0 },
    { -1.0,  1.0 },
    {  0.0, -1.0 },
    {  0.0,  1.0 },
    {  1.0, -1.0 }
  };

  const vector2& simplex_noise::grid(uint8_t i, uint8_t j) const {
    uint8_t index = i + m_perm.at(j);
    return s_gradients[index % 8];
  }

  static const double K = .366025403784438646763723170752; // (sqrt(3) - 1) / 2
  static const double C = .211324865405187117745425609748; // K / (1 + 2 * K)

  double simplex_noise::operator()(double x, double y) const {
    double s = (x + y) * K;
    double i = x + s;
    double j = y + s;

    i = std::floor(i);
    j = std::floor(j);

    double t = (i + j) * C;
    double X0 = i - t;
    double Y0 = j - t;

    double x0 = x - X0;
    double y0 = y - Y0;

    uint8_t i1 = 0;
    uint8_t j1 = 0;

    if (x0 > y0) {
      i1 = 1;
    } else {
      j1 = 1;
    }

    double x1 = x0 - i1 + C;
    double y1 = y0 - j1 + C;

    double x2 = x0 - 1 + 2.0 * C;
    double y2 = y0 - 1 + 2.0 * C;

    uint8_t ii = static_cast<uint8_t>(i);
    uint8_t jj = static_cast<uint8_t>(j);

    double res = 0.0;

    double d0 = 0.5 - x0*x0 - y0*y0;

    if (d0 > 0) {
      d0 *= d0;
      res += d0 * d0 * dot(grid(ii, jj), { x0, y0 });
    }

    double d1 = 0.5 - x1*x1 - y1*y1;

    if (d1 > 0) {
      d1 *= d1;
      res += d1 * d1 * dot(grid(ii + i1, jj + j1), { x1, y1 });
    }

    double d2 = 0.5 - x2*x2 - y2*y2;

    if (d2 > 0) {
      d2 *= d2;
      res += d2 * d2 * dot(grid(ii + 1, jj + 1), { x2, y2 });
    }

    return 60 * res;
  }

}
