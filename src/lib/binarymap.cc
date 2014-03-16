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
#include <mm/binarymap.h>

#include <fstream>
#include <iostream>
#include <queue>

namespace mm {

  void binarymap::output_to_pbm(const std::string& filename) const {
    std::ofstream file(filename);
    output_to_pbm(file);
  }

  void binarymap::output_to_pbm(std::ostream& file) const {
    file << "P1\n";
    file << this->width() << ' ' << this->height() << '\n';

    for (size_type y = 0; y < this->height(); ++y) {
      for (size_type x = 0; x < this->width(); ++x) {
        file << (this->get(x, y) ? '0' : '1') << ' ';
      }

      file << '\n';
    }
  }

  binarymap::size_type binarymap::walk(position start, std::function<void(position)> func) {
    size_type count = 0;

    std::queue<position> queue;
    queue.push(start);

    get(start) = true;

    while (!queue.empty()) {
      position pos = queue.front();

      if (func) {
        func(pos);
      }

      visit4neighbours(pos, [&queue](position neighbour, bool& visited) {
        if (visited) {
          return; // already visited
        }

        visited = true;
        queue.push(neighbour);
      });

      queue.pop();
      count++;
    }

    return count;
  }

}
