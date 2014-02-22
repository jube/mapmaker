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
#include <mm/reachability.h>

namespace mm {

  planemap<bool> reachability::operator()(const planemap<bool>& src) const {
    planemap<bool> map(src.width(), src.height(), false);

    size_type size2 = (m_size - 1) / 2;

    for (size_type x = 0; x < src.width() - size2; ++x) {
      for (size_type y = 0; y < src.height() - size2; ++y) {
        bool reachable = true;

        for (size_type i = 0; i < m_size && reachable; ++i) {
          for (size_type j = 0; j < m_size && reachable; ++j) {
            reachable = src(x+i, y+j);
          }
        }

        if (reachable) {
          for (size_type i = 0; i < m_size; ++i) {
            for (size_type j = 0; j < m_size; ++j) {
              map(x+i, y+j) = true;
            }
          }
        }
      }
    }

    return std::move(map);
  }

}