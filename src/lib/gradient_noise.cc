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
#include <mm/gradient_noise.h>

#include <cassert>
#include <cmath>

#include <mm/curve.h>

namespace mm {


  gradient_noise::gradient_noise(random_engine& engine, std::function<double(double)> curve)
  : m_curve(curve)
  {
    // generate gradients
    std::uniform_real_distribution<double> dist_grad(0.0, 2.0 * M_PI);
    for (auto& vec : m_gradients) {
      double angle = dist_grad(engine);
      vec.x = std::cos(angle);
      vec.y = std::sin(angle);
    }

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



  double gradient_noise::operator()(double x, double y) const {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double nw = dot(grid(qx    , qy    ), {rx      , ry      });
    double ne = dot(grid(qx + 1, qy    ), {rx - 1.0, ry      });
    double sw = dot(grid(qx    , qy + 1), {rx      , ry - 1.0});
    double se = dot(grid(qx + 1, qy + 1), {rx - 1.0, ry - 1.0});

    double n = lerp(nw, ne, m_curve(rx));
    double s = lerp(sw, se, m_curve(rx));

    return lerp(n, s, m_curve(ry));
  }

}
