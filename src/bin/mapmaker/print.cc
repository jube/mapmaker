#include "print.h"

#include <cstdio>

namespace mm {

  static int tabs = 0;

  void increment_indent() {
    tabs++;
  }

  void decrement_indent() {
    tabs--;
  }

  void print_indent() {
    for (int i = 0; i < tabs; ++i) {
      std::printf("\t");
    }
  }

}
