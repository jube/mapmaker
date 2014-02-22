#ifndef PRINT_H
#define PRINT_H

#define BEGIN_TYPE "\x1b[1m"
#define END_TYPE "\x1b[0m"

#define BEGIN_NAME "\x1b[31m"
#define END_NAME "\x1b[0m"

#define BEGIN_VALUE "\x1b[32m"
#define END_VALUE "\x1b[0m"

#define BEGIN_FILE "\x1b[33m"
#define END_FILE "\x1b[0m"

namespace mm {

  void increment_indent();
  void decrement_indent();
  void print_indent();

}

#endif // PRINT_H
