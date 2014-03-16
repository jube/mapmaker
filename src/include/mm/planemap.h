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
#ifndef MM_PLANEMAP_H
#define MM_PLANEMAP_H

#include <memory>
#include <stdexcept>

namespace mm {

  struct position {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    size_type x;
    size_type y;
  };

  struct fast_position {
    typedef typename position::size_type size_type;
    size_type index;
  };

  class position_iterator {
  public:
    position_iterator(const position_iterator&) = default;
    position_iterator& operator=(const position_iterator&) = default;

    fast_position operator*() {
      return { m_index };
    }

    position_iterator& operator++() {
      ++m_index;
      return *this;
    }

    bool operator!=(const position_iterator& other) const {
      return m_index != other.m_index;
    }

  private:
    friend class position_range;

    position_iterator(fast_position::size_type index)
    : m_index(index)
    {
    }

    fast_position::size_type m_index;
  };

  class position_range {
  public:
    position_range(const position_range&) = default;
    position_range& operator=(const position_range&) = default;

    position_iterator begin() {
      return { m_b };
    }

    position_iterator end() {
      return { m_e };
    }

  private:
    template<class T, class Allocator>
    friend class planemap;

    position_range(fast_position::size_type b, fast_position::size_type e)
    : m_b(b)
    , m_e(e)
    {
    }

    fast_position::size_type m_b;
    fast_position::size_type m_e;
  };


  class index_iterator {
  public:
    typedef typename position::size_type size_type;

    index_iterator(const index_iterator&) = default;
    index_iterator& operator=(const index_iterator&) = default;

    size_type operator*() {
      return m_index;
    }

    index_iterator& operator++() {
      ++m_index;
      return *this;
    }

    bool operator!=(const index_iterator& other) const {
      return m_index != other.m_index;
    }

  private:
    friend class index_range;

    index_iterator(size_type index)
    : m_index(index)
    {
    }

    size_type m_index;
  };

  class index_range {
  public:
    index_range(const index_range&) = default;
    index_range& operator=(const index_range&) = default;

    index_iterator begin() {
      return { m_b };
    }

    index_iterator end() {
      return { m_e };
    }

  private:
    template<class T, class Allocator>
    friend class planemap;

    index_range(index_iterator::size_type b, index_iterator::size_type e)
    : m_b(b)
    , m_e(e)
    {
    }

    index_iterator::size_type m_b;
    index_iterator::size_type m_e;
  };

  struct size_only_t {
  };

  constexpr size_only_t size_only = size_only_t();

  template<class T, class Allocator =  std::allocator<T>>
  class planemap {
    static_assert(std::is_default_constructible<T>::value, "T should be default constructible");
    static_assert(std::is_copy_constructible<T>::value, "T should be copy constructible");
  public:
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename position::size_type size_type;
    typedef typename position::difference_type difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename std::allocator_traits<Allocator>::pointer pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;

    planemap()
    : m_allocator(Allocator())
    , m_w(0)
    , m_h(0)
    , m_content(nullptr)
    {
    }

    planemap(size_type w, size_type h)
    : m_allocator(Allocator())
    , m_w(w)
    , m_h(h)
    , m_content(m_allocator.allocate(w * h))
    {
      size_type end =  m_w * m_h;
      for (size_type i = 0; i < end; ++i) {
        m_allocator.construct(m_content + i);
      }
    }

    planemap(size_type w, size_type h, const T& value)
    : m_allocator(Allocator())
    , m_w(w)
    , m_h(h)
    , m_content(m_allocator.allocate(w * h))
    {
      size_type end =  m_w * m_h;
      for (size_type i = 0; i < end; ++i) {
        m_allocator.construct(m_content + i, value);
      }
    }

    planemap(const planemap& other)
    : m_allocator(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.m_allocator))
    , m_w(other.m_w)
    , m_h(other.m_h)
    , m_content(m_allocator.allocate(m_h * m_w))
    {
      size_type end = m_w * m_h;
      for (size_type i = 0; i < end; ++i) {
        m_allocator.construct(m_content + i, other.m_content[i]);
      }
    }

    template<typename U>
    planemap(size_only_t, const planemap<U>& other)
    : planemap(other.width(), other.height())
    {
    }

    planemap(planemap&& other)
    : m_allocator(std::move(other.m_allocator))
    , m_w(other.m_w)
    , m_h(other.m_h)
    , m_content(other.m_content)
    {
      other.m_h = other.m_w = 0;
      other.m_content = nullptr;
    }

    ~planemap() {
      clear();
    }

    planemap& operator=(const planemap& other) {
      if (this == &other) {
        return *this;
      }

      clear();
      m_allocator = other.m_allocator;
      m_w = other.m_w;
      m_h = other.m_h;
      m_content = m_allocator.allocate(m_w * m_h);
      size_type end = m_w * m_h;

      for (size_type i = 0; i < end; ++i) {
        m_allocator.construct(m_content + i, other.m_content[i]);
      }

      return *this;
    }

    planemap& operator=(planemap&& other) {
      if (this == &other) {
        return *this;
      }

      clear();
      m_allocator = std::move(other.m_allocator);
      m_w = other.m_w;
      m_h = other.m_h;
      m_content = other.m_content;

      other.m_h = 0;
      other.m_w = 0;
      other.m_content = nullptr;

      return *this;
    }

    allocator_type get_allocator() const {
      return m_allocator;
    }

    // element access

    reference at(size_type x, size_type y) {
      if (x >= m_w || y >= m_h) {
        throw std::out_of_range("planemap::at");
      }

      return get(x, y);
    }

    reference at(position pos) {
      return at(pos.x, pos.y);
    }

    reference at(fast_position pos) {
      if (pos.index >= m_w * m_h) {
        throw std::out_of_range("planemap::at");
      }

      return m_content[pos.index];
    }

    const_reference at(size_type x, size_type y) const {
      if (x >= m_w || y >= m_h) {
        throw std::out_of_range("planemap::at");
      }

      return get(x, y);
    }

    const_reference at(position pos) const {
      return at(pos.x, pos.y);
    }

    const_reference at(fast_position pos) const {
      if (pos.index >= m_w * m_h) {
        throw std::out_of_range("planemap::at");
      }

      return m_content[pos.index];
    }

    reference operator()(size_type x, size_type y) {
      return get(x, y);
    }

    reference operator()(position pos) {
      return get(pos.x, pos.y);
    }

    reference operator()(fast_position pos) {
      return m_content[pos.index];
    }

    const_reference operator()(size_type x, size_type y) const {
      return get(x, y);
    }

    const_reference operator()(position pos) const {
      return get(pos.x, pos.y);
    }

    const_reference operator()(fast_position pos) const {
      return m_content[pos.index];
    }

    // capacity

    bool empty() const {
      return m_content == nullptr;
    }

    size_type width() const {
      return m_w;
    }

    size_type height() const {
      return m_h;
    }

    // modifiers

    void clear() {
      if (m_content) {
        size_type end = m_w * m_h;
        for (size_type i = 0; i < end; ++i) {
          m_allocator.destroy(m_content + i);
        }

        m_allocator.deallocate(m_content, m_w * m_h);
        m_w = m_h = 0;
        m_content = nullptr;
      }
    }

    void swap(planemap& other) {
      std::swap(m_allocator, other.m_allocator);
      std::swap(m_w, other.m_w);
      std::swap(m_h, other.m_h);
      std::swap(m_content, other.m_content);
    }

    // visitors

    template<typename Func>
    void visit4neighbours(size_type x, size_type y, Func func) const {
      if (x > 0) {
        position pos{x - 1, y};
        func(pos, get(pos));
      }

      if (x < m_w - 1) {
        position pos{x + 1, y};
        func(pos, get(pos));
      }

      if (y > 0) {
        position pos{x, y - 1};
        func(pos, get(pos));
      }

      if (y < m_h - 1) {
        position pos{x, y + 1};
        func(pos, get(pos));
      }
    }

    template<typename Func>
    void visit4neighbours(position pos, Func func) const {
      visit4neighbours(pos.x, pos.y, func);
    }


    template<typename Func>
    void visit8neighbours(size_type x, size_type y, Func func) const {
      for (int i = -1; i <= 1; ++i) {
        if (x == 0 && i == -1) {
          continue;
        }

        if (x == m_w - 1 && i == 1) {
          continue;
        }

        for (int j = -1; j <= 1; ++j) {
          if (y == 0 && j == -1) {
            continue;
          }

          if (y == m_h - 1 && j == 1) {
            continue;
          }

          if (i != 0 || j != 0) {
            position pos{x + i, y + j};
            func(pos, get(pos));
          }
        }
      }
    }

    template<typename Func>
    void visit8neighbours(position pos, Func func) const {
      visit8neighbours(pos.x, pos.y, func);
    }


    // modifiers

    void reset(value_type value) {
      size_type end = m_w * m_h;

      for (size_type i = 0; i < end; ++i) {
        m_content[i] = value;
      }
    }

    // utils

    position_range positions() const {
      return { 0, m_w * m_h };
    }

    position to_position(fast_position pos) const {
      return { pos.index / m_h, pos.index % m_h };
    }

    index_range x_range() const {
      return { 0, m_w };
    }

    index_range y_range() const {
      return { 0, m_h };
    }

  protected:

    reference get(size_type x, size_type y) const {
      return m_content[x * m_h + y];
    }

    reference get(position pos) const {
      return get(pos.x, pos.y);
    }

  private:
    allocator_type m_allocator;
    size_type m_w;
    size_type m_h;
    pointer m_content;
  };

}

#endif // MM_PLANEMAP_H
