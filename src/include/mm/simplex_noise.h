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
#ifndef MM_SIMPLEX_NOISE_H
#define MM_SIMPLEX_NOISE_H

#include <cstdint>
#include <array>
#include <functional>

#include <mm/vector2.h>
#include <mm/random.h>

namespace mm {

  class simplex_noise {
  public:
    simplex_noise(random_engine& engine);

    double operator()(double x, double y) const;

  private:
    std::array<uint8_t, 256> m_perm;

    const vector2& grid(uint8_t i, uint8_t j) const;

  };

}

#endif // MM_SIMPLEX_NOISE_H
