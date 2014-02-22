#ifndef MM_VALUE_NOISE_H
#define MM_VALUE_NOISE_H

#include <cstdint>
#include <array>
#include <functional>

#include <mm/random.h>

namespace mm {

  class value_noise {
  public:
    value_noise(random_engine& engine, std::function<double(double)> curve);

    double operator()(double x, double y) const;

  private:
    std::function<double(double)> m_curve;
    std::array<double, 256> m_values;
    std::array<uint8_t, 256> m_perm;

    double grid(uint8_t i, uint8_t j) const {
      uint8_t index = i + m_perm.at(j);
      return m_values.at(index);
    }

  };

}


#endif // MM_VALUE_NOISE_H
