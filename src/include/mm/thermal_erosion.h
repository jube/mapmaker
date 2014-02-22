#ifndef MM_THERMAL_EROSION_H
#define MM_THERMAL_EROSION_H

#include <mm/heightmap.h>

namespace mm {

  class thermal_erosion {
  public:
    typedef std::size_t size_type;

    thermal_erosion(size_type iterations, double talus, double fraction)
    : m_iterations(iterations), m_talus(talus), m_fraction(fraction)
    {
    }

    heightmap operator()(const heightmap& src) const;

  private:
    size_type m_iterations;
    double m_talus;
    double m_fraction;
  };


}

#endif // MM_THERMAL_EROSION_H
