#ifndef MM_GRADIENT_NOISE_H
#define MM_GRADIENT_NOISE_H

#include <cstdint>
#include <array>
#include <functional>

#include <mm/vector2.h>
#include <mm/random.h>

namespace mm {

  class gradient_noise {
  public:
    gradient_noise(random_engine& engine, std::function<double(double)> curve);

    double operator()(double x, double y) const;

  private:
    std::function<double(double)> m_curve;
    std::array<vector2, 256> m_gradients;
    std::array<uint8_t, 256> m_perm;

    const vector2& grid(uint8_t i, uint8_t j) const {
      uint8_t index = i + m_perm.at(j);
      return m_gradients.at(index);
    }

  };

}


#endif // MM_GRADIENT_NOISE_H
