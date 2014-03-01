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
#ifndef MM_COLOR_H
#define MM_COLOR_H

#include <cstdint>

namespace mm {

  class color {
  public:
    static constexpr uint8_t max = 255;

    constexpr color()
    : m_r(0), m_g(0), m_b(0), m_a(max)
    {
    }

    constexpr color(uint8_t r, uint8_t g, uint8_t b)
    : m_r(r), m_g(g), m_b(b), m_a(max)
    {
    }

    constexpr color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : m_r(r), m_g(g), m_b(b), m_a(a)
    {
    }

    constexpr color(const color& other) = default;
    color& operator=(const color& other) = default;

    uint8_t red_channel() const {
      return m_r;
    }

    uint8_t green_channel() const {
      return m_g;
    }

    uint8_t blue_channel() const {
      return m_b;
    }

    uint8_t alpha_channel() const {
      return m_a;
    }

    void set_alpha_channel(uint8_t a) {
      m_a = a;
    }

    static constexpr color black() {
      return {0, 0, 0};
    }

    static constexpr color white() {
      return {max, max, max};
    }

    static constexpr color red() {
      return {max, 0, 0};
    }

    static constexpr color green() {
      return {0, max, 0};
    }

    static constexpr color blue() {
      return {0, 0, max};
    }

    static constexpr color cyan() {
      return {0, max, max};
    }

    static constexpr color magenta() {
      return {max, 0, max};
    }

    static constexpr color yellow() {
      return {max, max, 0};
    }

    static constexpr color transparent() {
      return {0, 0, 0, 0};
    }

    friend constexpr bool operator==(const color& lhs, const color& rhs);

  private:
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
    uint8_t m_a;
  };

  constexpr bool operator==(const color& lhs, const color& rhs) {
    return (lhs.m_r == rhs.m_r) && (lhs.m_g == rhs.m_g) && (lhs.m_b == rhs.m_b) && (lhs.m_a == rhs.m_a);
  }

}

#endif // MM_COLOR_H
