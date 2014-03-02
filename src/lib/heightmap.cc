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
#include <mm/heightmap.h>

#include <cassert>
#include <iostream>

namespace mm {

  heightmap heightmap::submap(size_type x, size_type y, size_type w, size_type h) const {
    if (x + w > this->width()) {
      w = this->width() - x;
    }

    if (y + h > this->height()) {
      h = this->height() - y;
    }

    heightmap sub(w, h);

    for (size_type i = 0; i < w; ++i) {
      for (size_type j = 0; j < h; ++j) {
        sub(i, j) = this->get(x + i, y + j);
      }
    }

    return std::move(sub);
  }

  #define WHITE 65535

  void heightmap::output_to_pgm(std::ostream& file) const {
    file << "P2\n";
    file << this->width() << ' ' << this->height() << '\n';
    file << WHITE << '\n';

    for (size_type y = 0; y < this->height(); ++y) {
      for (size_type x = 0; x < this->width(); ++x) {
        unsigned value = static_cast<unsigned>(this->get(x, y) * WHITE);
        assert(0 <= value && value <= WHITE);
        file << value << ' ';
      }

      file << '\n';
    }
  }

}
