#ifndef MM_COLOR_RAMP_H
#define MM_COLOR_RAMP_H

#include <map>

#include <mm/color.h>

namespace mm {

  class color_ramp {
  public:
    color_ramp()
    : m_min(0.0), m_max(0.0)
    {
    }

    bool empty() const {
      return m_map.empty();
    }

    void add_color_stop(double offset, const color& c);

    color get_color(double offset) const;

  private:
    double m_min;
    double m_max;
    std::map<double, color> m_map;
  };

}

#endif // MM_COLOR_RAMP_H
