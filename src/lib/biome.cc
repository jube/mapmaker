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
#include <mm/biome.h>

namespace mm {

  biome& biome::add_climate(range altitude_range, range humidity_range, bool water) {
    m_climates.emplace_back(climate{altitude_range, humidity_range, water});
    return *this;
  }

  static bool climate_match(const climate& c, double  altitude, double humidity, bool water) {
    if (altitude < c.altitude.min || c.altitude.max < altitude) {
      return false;
    }

    if (humidity < c.humidity.min || c.humidity.max < humidity) {
      return false;
    }

    return water == c.water;
  }

  bool biome::match(double altitude, double humidity, bool water) const {
    for (auto climate : m_climates) {
      if (climate_match(climate, altitude, humidity, water)) {
        return true;
      }
    }

    return false;
  }

  bool biome::has_higher_priority(const biome& other) const {
//     if (m_water != other.m_water) {
//       return m_water;
//     }
//
//     if (m_altitude_range.min > other.m_altitude_range.min) {
//       return true;
//     }
//
//     if (m_humidity_range.min > other.m_humidity_range.min) {
//       return true;
//     }

    return false;
  }

}
