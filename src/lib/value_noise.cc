#include <mm/value_noise.h>

#include <cassert>
#include <cmath>

#include <mm/curve.h>

namespace mm {


  value_noise::value_noise(random_engine& engine, std::function<double(double)> curve)
  : m_curve(curve)
  {
    // generate values
    std::uniform_real_distribution<double> dist_value(0.0, 1.0);
    for (auto& value : m_values) {
      value = dist_value(engine);
    }

    // initialize permutation
    for (uint8_t i = 0; i < 255; ++i) {
      m_perm[i] = i;
    }

    m_perm[255] = 255; // to avoid an infinite loop in the previous for loop

    // generate permutation
    std::uniform_int_distribution<uint8_t> dist_perm(0, 255);
    for (unsigned i = 0; i < 2560; ++i) {
      uint8_t j = dist_perm(engine);
      uint8_t k = dist_perm(engine);
      std::swap(m_perm[j], m_perm[k]);
    }
  }



  double value_noise::operator()(double x, double y) const {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double nw = grid(qx    , qy    );
    double ne = grid(qx + 1, qy    );
    double sw = grid(qx    , qy + 1);
    double se = grid(qx + 1, qy + 1);

    double n = lerp(nw, ne, m_curve(rx));
    double s = lerp(sw, se, m_curve(rx));

    return lerp(n, s, m_curve(ry));
  }

}
