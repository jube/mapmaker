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
#include <mm/color_ramp.h>

#include <cassert>
#include <algorithm>
#include <string>

#include <mm/curve.h>
#include <mm/utils.h>

namespace mm {

  static color lerp(const color& lhs, const color& rhs, double t) {
    return {
      lerp(lhs.red_channel(), rhs.red_channel(), t),
      lerp(lhs.green_channel(), rhs.green_channel(), t),
      lerp(lhs.blue_channel(), rhs.blue_channel(), t),
      lerp(lhs.alpha_channel(), rhs.alpha_channel(), t)
    };
  }

  void color_ramp::add_color_stop(double offset, const color& c) {
    if (empty()) {
      m_min = offset;
      m_max = offset;
    } else {
      if (offset < m_min) {
        m_min = offset;
      }

      if (offset > m_max) {
        m_max = offset;
      }
    }

    m_map.emplace(offset, c);
  }

  color color_ramp::compute_color(double offset) const {
    if (offset < m_min || offset > m_max) {
      return color();
    }

    color c1, c2;
    double t1 = 0.0;
    double t2 = 0.0;
    for (auto value : m_map) {
      if (value.first <= offset) {
        t1 = value.first;
        c1 = value.second;
      } else {
        t2 = value.first;
        c2 = value.second;
        break;
      }
    }

    return lerp(c1, c2, (offset - t1) / (t2 - t1));
  }

  tileset color_ramp::compute_tileset() const {
    std::vector<double> limits;

    double prev = -1.0;
    for (auto value : m_map) {
      if (prev < 0) {
        prev = value.first;
      } else {
        double curr = value.first;
        double limit = (prev + curr) / 2.0;
        prev = curr;
        limits.push_back(limit);
      }
    }
    limits.push_back(1.0);
    assert(m_map.size() == limits.size());
    assert(std::is_sorted(limits.begin(), limits.end()));

    tileset set;
    set.add_terrain({ "River", { 19, 149, 255 }, { 0.5, 1 }, { 0, 1 }, true }); // river

    prev = 0.0;
    std::size_t index = 0;
    for (auto value : m_map) {
      std::string name = "Biome #" + std::to_string(index);

      double curr = limits[index];

      if (prev < 0.5 && 0.5 < curr) {
        set.add_terrain({ name, value.second, { prev, 0.5 }, { 0, 1 }, true });
        set.add_terrain({ name, value.second, { 0.5, curr }, { 0, 1 }, false });
      } else {
        set.add_terrain({ name, value.second, { prev, curr }, { 0, 1 }, curr < 0.5 });
      }

      prev = curr;
      index++;
    }

    return set;
  }


  color_ramp color_ramp::basic() {
    // see: http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
    color_ramp ramp;
    ramp.add_color_stop(0.000, {  2,  43,  68}); // very dark blue: deep water
    ramp.add_color_stop(0.250, {  9,  62,  92}); // dark blue: water
    ramp.add_color_stop(0.490, { 17,  82, 112}); // blue: shallow water
    ramp.add_color_stop(0.500, { 69, 108, 118}); // light blue: shore
    ramp.add_color_stop(0.501, { 42, 102,  41}); // green: grass
    ramp.add_color_stop(0.750, {115, 128,  77}); // light green: veld
    ramp.add_color_stop(0.850, {153, 143,  92}); // brown: tundra
    ramp.add_color_stop(0.950, {179, 179, 179}); // grey: rocks
    ramp.add_color_stop(1.000, {255, 255, 255}); // white: snow

    return ramp;
  }

}
