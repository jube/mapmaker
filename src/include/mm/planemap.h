
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

    const_reference at(size_type x, size_type y) const {
      if (x >= m_w || y >= m_h) {
        throw std::out_of_range("planemap::at");
      }

      return get(x, y);
    }

    const_reference at(position pos) const {
      return at(pos.x, pos.y);
    }

    reference operator()(size_type x, size_type y) {
      return get(x, y);
    }

    reference operator()(position pos) {
      return get(pos.x, pos.y);
    }

    const_reference operator()(size_type x, size_type y) const {
      return get(x, y);
    }

    const_reference operator()(position pos) const {
      return get(pos.x, pos.y);
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

  protected:

    reference get(size_type x, size_type y) const {
      return m_content[x * m_h + y];
    }

  private:
    allocator_type m_allocator;
    size_type m_w;
    size_type m_h;
    pointer m_content;
  };

}

#endif // MM_PLANEMAP_H
