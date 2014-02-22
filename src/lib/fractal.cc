#include <mm/fractal.h>

namespace mm {

  heightmap fractal::operator()(random_engine& engine, size_type width, size_type height) const {
    heightmap map(width, height);

    for (size_type x = 0; x < width; ++x) {
      for (size_type y = 0; y < height; ++y) {
        double value = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;

        const double xf = static_cast<double>(x) / static_cast<double>(width);
        const double yf = static_cast<double>(y) / static_cast<double>(height);

        for (size_type k = 0; k < m_octaves; ++k) {
          value += m_noise(xf * frequency, yf * frequency) * amplitude;
          frequency *= m_lacunarity;
          amplitude *= m_persistence;
        }

        map(x, y) = value;
      }
    }

    return std::move(map);
  }

}
