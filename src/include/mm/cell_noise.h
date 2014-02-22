#ifndef MM_CELL_NOISE_H
#define MM_CELL_NOISE_H

#include <functional>
#include <vector>

#include <mm/vector2.h>
#include <mm/random.h>

namespace mm {

  class cell_noise {
  public:
    typedef std::size_t size_type;

    cell_noise(random_engine& engine, size_type count, std::function<double(const vector2&, const vector2&)> distance, std::vector<double> coeffs);

    double operator()(double x, double y);

  private:
    size_type m_count;
    std::function<double(const vector2&, const vector2&)> m_distance;
    std::vector<double> m_coeffs;
    std::vector<vector2> m_cells;
  };

}

#endif // MM_CELL_NOISE_H
