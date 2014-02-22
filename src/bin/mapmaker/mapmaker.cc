/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <cstdio>

#include <yaml-cpp/yaml.h>

#include "exception.h"
#include "process.h"

static void usage() {
  std::printf("Usage: mapmaker <file>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage();
    return 0;
  }

  try {

    YAML::Node node = YAML::LoadFile(argv[1]);

    auto map = mm::process_generator(node);
    map = mm::process_modifiers(map, node);
    mm::process_finalizer(map, node);

  } catch (std::exception& ex) {
    std::printf("Error: %s\n", ex.what());
    return 1;
  }

  return 0;
}
