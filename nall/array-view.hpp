#pragma once

#include <nall/iterator.hpp>

namespace nall {

struct string;
template<typename T> struct vector;
template<typename T> struct array;

template<typename T> struct array_view {
  using type = array_view;

  inline array_view() {
    _data = nullptr;
    _size = 0;
  }

  inline array_view(const void* data, uint64_t size) {
    _data = (const T*)data;
    _size = (uint)size;
  }

  inline explicit operator bool() const { return _data && _size > 0; }
  inline operator const T*() const { return _data; }

  inline auto operator++() -> type& { _data++; _size--; return *this; }
  inline auto operator--() -> type& { _data--; _size++; return *this; }

  inline auto operator++(int) -> type { auto copy = *this; ++(*this); return copy; }
  inline auto operator--(int) -> type { auto copy = *this; --(*this); return copy; }

  inline auto operator[](uint index) const -> const T& {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(index >= _size) throw out_of_bounds{};
    #endif
    return _data[index];
  }

  inline auto operator()(uint index, const T& fallback = {}) const -> T {
    if(index >= _size) return fallback;
    return _data[index];
  }

  template<typename U = T> inline auto data() const -> const U* { return (const U*)_data; }
  template<typename U = T> inline auto size() const -> uint64_t { return _size * sizeof(T) / sizeof(U); }

  inline auto begin() const -> iterator_const<T> { return {_data, (uint)0}; }
  inline auto end() const -> iterator_const<T> { return {_data, (uint)_size}; }

  inline auto rbegin() const -> reverse_iterator_const<T> { return {_data, (uint)_size - 1}; }
  inline auto rend() const -> reverse_iterator_const<T> { return {_data, (uint)-1}; }

protected:
  const T* _data;
  int _size;
};

}
