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
#include <mm/hull.h>

#include <cassert>
#include <algorithm>
#include <iostream>

namespace mm {
  static bool operator==(const position& lhs, const position& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  static bool operator!=(const position& lhs, const position& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
  }

  static bool position_compare(const position& lhs, const position& rhs) {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
  }

  typedef std::pair<position, position> edge;

  static bool edge_compare(const edge& lhs, const edge& rhs) {
    return position_compare(lhs.first, rhs.first) || (lhs.first == rhs.first && position_compare(lhs.second, rhs.second));
  }

  struct compare_border_edges {
    bool operator()(const edge& lhs, const position& rhs) {
      return position_compare(lhs.first, rhs);
    }

    bool operator()(const position& lhs, const edge& rhs) {
      return position_compare(lhs, rhs.first);
    }
  };

  static bool turn_left(const edge& current, const edge& next) {
    assert(current.second == next.first);

    if (current.first.x < current.second.x) {
      return next.first.y > next.second.y;
    }

    if (current.first.x > current.second.x) {
      return next.first.y < next.second.y;
    }

    if (current.first.y < current.second.y) {
      return next.first.x < next.second.x;
    }

    if (current.first.y > current.second.y) {
      return next.first.x > next.second.x;
    }

    assert(false);
    return false;
  }

  std::vector<std::vector<position>> hull::operator()(const binarymap& src) const {
    std::vector<std::vector<position>> hulls;

    /*
     * determine all the edges
     */
    std::vector<edge> edges;

    for (auto x : src.x_range()) {
      for (auto y : src.y_range()) {
        if (!src(x, y)) {
          continue;
        }

        position nw{x, y};
        position ne{(x + 1), y};
        position sw{x, (y + 1)};
        position se{(x + 1), (y + 1)};

        edges.emplace_back(std::make_pair(nw, ne));
        edges.emplace_back(std::make_pair(ne, se));
        edges.emplace_back(std::make_pair(se, sw));
        edges.emplace_back(std::make_pair(sw, nw));
      }
    }

    std::sort(edges.begin(), edges.end(), edge_compare);

    /*
     * remove the double edges
     */
    std::vector<edge> double_edges;

    for (auto e : edges) {
      edge reverse{e.second, e.first};

      if (std::binary_search(edges.begin(), edges.end(), reverse, edge_compare)) {
        double_edges.push_back(e);
      }
    }

    std::sort(double_edges.begin(), double_edges.end(), edge_compare);

    std::vector<edge> border_edges(edges.size() - double_edges.size());
    std::set_difference(edges.begin(), edges.end(), double_edges.begin(), double_edges.end(), border_edges.begin(), edge_compare);

    /*
     * reconstruct the borders
     */
    std::vector<edge> used_edges;

    while (!border_edges.empty()) {
      edge current = border_edges.front();
      position first = current.first;

      std::vector<position> new_hull;
      new_hull.push_back(first);
      used_edges.push_back(current);

      while (current.second != first) {
        auto range = std::equal_range(border_edges.begin(), border_edges.end(), current.second, compare_border_edges());

        auto count = std::distance(range.first, range.second);

        if (count == 1) {
          current = *range.first;
        } else {
          assert(count == 2);

          for (auto it = range.first; it != range.second; ++it) {
            auto next = *it;
            if (turn_left(current, next)) {
              current = next;
              break;
            }
          }
        }

        new_hull.push_back(current.first);
        used_edges.push_back(current);
      }

      hulls.push_back(std::move(new_hull));

      assert(std::is_sorted(border_edges.begin(), border_edges.end(), edge_compare));

      edges.clear();
      edges.resize(border_edges.size() - used_edges.size());

      std::sort(used_edges.begin(), used_edges.end(), edge_compare);
      std::set_difference(border_edges.begin(), border_edges.end(), used_edges.begin(), used_edges.end(), edges.begin(), edge_compare);

      std::swap(border_edges, edges);
      used_edges.clear();
    }

    return hulls;
  }

}
