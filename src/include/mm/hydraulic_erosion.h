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
