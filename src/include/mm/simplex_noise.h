#ifndef MM_SIMPLEX_NOISE_H
#define MM_SIMPLEX_NOISE_H

#include <cstdint>
#include <array>
#include <functional>

#include <mm/vector2.h>
#include <mm/random.h>

namespace mm {

  class simplex_noise {
  public:
    simplex_noise(random_engine& engine);

    double operator()(double x, double y) const;

  private:
    std::array<uint8_t, 256> m_perm;

    const vector2& grid(uint8_t i, uint8_t j) const;

  };

}


#endif // MM_SIMPLEX_NOISE_H
