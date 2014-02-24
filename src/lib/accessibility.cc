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
#include <mm/accessibility.h>

#include <queue>

namespace mm {

  typedef typename planemap<bool>::size_type size_type;

  template<typename Func>
  static size_type walk(planemap<bool>& unknown, position start, Func func) {
    size_type count = 0;
    std::queue<position> q;
    q.push(start);

    while (!q.empty()) {
      position pos = q.front();
      q.pop();
      count++;

      if (pos.x > 0) {
        position neighbour{pos.x - 1, pos.y};
        if (unknown(neighbour)) {
          unknown(neighbour) = false;
          func(neighbour);
          q.push(neighbour);
        }
      }

      if (pos.x < unknown.width() - 1) {
        position neighbour{pos.x + 1, pos.y};
        if (unknown(neighbour)) {
          unknown(neighbour) = false;
          func(neighbour);
          q.push(neighbour);
        }
      }

      if (pos.y > 0) {
        position neighbour{pos.x, pos.y - 1};
        if (unknown(neighbour)) {
          unknown(neighbour) = false;
          func(neighbour);
          q.push(neighbour);
        }
      }

      if (pos.y < unknown.height() - 1) {
        position neighbour{pos.x, pos.y + 1};
        if (unknown(neighbour)) {
          unknown(neighbour) = false;
          func(neighbour);
          q.push(neighbour);
        }
      }
    }

    return count;
  }

  planemap<bool> accessibility::operator()(const planemap<bool>& src) {
    planemap<bool> unknown(src);

    position best_pos{0, 0};
    size_type best_count = 0;

    for (size_type x = 0; x < unknown.width(); ++x) {
      for (size_type y = 0; y < unknown.height(); ++y) {
        position pos{x, y};

        if (!unknown(pos)) {
          continue;
        }

        unknown(pos) = false;

        size_type count = walk(unknown, pos, [](position neighbour) { });

        if (count > best_count) {
          best_pos = pos;
          best_count = count;
        }
      }
    }

    unknown = src;

    planemap<bool> map(src.width(), src.height(), false);

    map(best_pos) = true;
    walk(unknown, best_pos, [&map](position neighbour) {
      map(neighbour) = true;
    });

    return std::move(map);
  }

}
