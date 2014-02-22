#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <stdexcept>

namespace mm {

  class bad_structure : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
  };

}

#endif // EXCEPTION_H
