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
#include <mm/cell_noise.h>

#include <cassert>
#include <algorithm>

namespace mm {

#define NCELLS 32

  cell_noise::cell_noise(random_engine& engine, size_type count, std::function<double(const vector2&, const vector2&)> distance, std::vector<double> coeffs)
  : m_count(count)
  , m_distance(distance)
  , m_coeffs(std::move(coeffs))
  {

    // generate cells
    m_cells.reserve(m_count * 4);
    std::uniform_real_distribution<double> dist_coord(0.0, 1.0);
    for (size_type i = 0; i < m_count; ++i) {
      auto x = dist_coord(engine);
      auto y = dist_coord(engine);

      m_cells.push_back({x, y});

      if (x < 0.5) {
        if (y < 0.5) {
          m_cells.push_back({x + 1.0, y      });
          m_cells.push_back({x      , y + 1.0});
          m_cells.push_back({x + 1.0, y + 1.0});
        } else {
          m_cells.push_back({x + 1.0, y      });
          m_cells.push_back({x      , y - 1.0});
          m_cells.push_back({x + 1.0, y - 1.0});
        }
      } else {
        if (y < 0.5) {
          m_cells.push_back({x - 1.0, y      });
          m_cells.push_back({x      , y + 1.0});
          m_cells.push_back({x - 1.0, y + 1.0});
        } else {
          m_cells.push_back({x - 1.0, y      });
          m_cells.push_back({x      , y - 1.0});
          m_cells.push_back({x - 1.0, y - 1.0});
        }
      }
    }

    // some sanity checks
    if (m_coeffs.empty()) {
      m_coeffs.push_back(1.0);
    }

    if (m_coeffs.size() > m_cells.size()) {
      m_coeffs.resize(m_cells.size());
    }

  }

  double cell_noise::operator()(double x, double y) {
    double rx = std::fmod(x, 1);
    double ry = std::fmod(y, 1);

    auto size = m_coeffs.size();

    vector2 here{rx, ry};

    std::partial_sort(m_cells.begin(), m_cells.begin() + size, m_cells.end(), [&here, this](const vector2& lhs, const vector2& rhs) {
      return m_distance(here, lhs) < m_distance(here, rhs);
    });

    double value = 0.0;

    for (decltype(size) i = 0; i < size; ++i) {
      value += m_coeffs[i] * m_distance(here, m_cells[i]);
    }

    return value;
  }


}
