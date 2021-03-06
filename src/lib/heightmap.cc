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
#include <fstream>
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

    return sub;
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

  void heightmap::output_to_pgm(const std::string& filename) const {
    std::ofstream file(filename);
    output_to_pgm(file);
  }

  heightmap heightmap::input_from_pgm(std::istream& file) {
    std::string header;
    file >> header;
    assert(header == "P2");

    size_type width, height;
    file >> width >> height;

    unsigned white;
    file >> white;

    heightmap map(width, height);

    for (size_type y = 0; y < height; ++y) {
      for (size_type x = 0; x < width; ++x) {
        unsigned value;
        file >> value;
        assert(0 <= value && value <= white);

        map(x, y) = static_cast<double>(value) / white;
      }
    }

    return map;
  }


  heightmap heightmap::input_from_pgm(const std::string& filename) {
    std::ifstream file(filename);
    return input_from_pgm(file);
  }

}
