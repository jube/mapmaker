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
#ifndef MM_CELL_NOISE_H
#define MM_CELL_NOISE_H

#include <functional>
#include <vector>

#include <mm/vector2.h>
#include <mm/random.h>

namespace mm {

  class cell_noise {
  public:
    typedef std::size_t size_type;

    cell_noise(random_engine& engine, size_type count, std::function<double(const vector2&, const vector2&)> distance, std::vector<double> coeffs);

    double operator()(double x, double y);

  private:
    size_type m_count;
    std::function<double(const vector2&, const vector2&)> m_distance;
    std::vector<double> m_coeffs;
    std::vector<vector2> m_cells;
  };

}

#endif // MM_CELL_NOISE_H
