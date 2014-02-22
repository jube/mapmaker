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
#ifndef MM_HYDRAULIC_EROSION_H
#define MM_HYDRAULIC_EROSION_H

#include <mm/heightmap.h>

namespace mm {

  class hydraulic_erosion {
  public:
    typedef std::size_t size_type;

    hydraulic_erosion(size_type iterations, double rain_amount, double solubility, double evaporation, double capacity)
    : m_iterations(iterations), m_rain_amount(rain_amount), m_solubility(solubility), m_evaporation(evaporation), m_capacity(capacity)
    {
    }

    heightmap operator()(const heightmap& src) const;

  private:
    size_type m_iterations;
    double m_rain_amount;
    double m_solubility;
    double m_evaporation;
    double m_capacity;

  };

}

#endif // MM_HYDRAULIC_EROSION_H
