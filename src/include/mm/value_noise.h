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
#ifndef MM_VALUE_NOISE_H
#define MM_VALUE_NOISE_H

#include <cstdint>
#include <array>
#include <functional>

#include <mm/random.h>

namespace mm {

  class value_noise {
  public:
    value_noise(random_engine& engine, std::function<double(double)> curve);

    double operator()(double x, double y) const;

  private:
    std::function<double(double)> m_curve;
    std::array<double, 256> m_values;
    std::array<uint8_t, 256> m_perm;

    double grid(uint8_t i, uint8_t j) const {
      uint8_t index = i + m_perm.at(j);
      return m_values.at(index);
    }

  };

}


#endif // MM_VALUE_NOISE_H
