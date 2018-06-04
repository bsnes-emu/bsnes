#pragma once
#define DEBUG

#include <nall/range.hpp>

namespace nall {

template<typename T> struct array;

//usage: int x[256] => array<int, 256>
template<typename T, uint Size> struct array<T[Size]> {
  array() = default;

  array(const initializer_list<T>& source) {
    uint index = 0;
    for(auto& value : source) {
      operator[](index++) = value;
    }
  }

  alwaysinline auto operator[](uint index) -> T& {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(index >= Size) throw out_of_bounds{};
    #endif
    return values[index];
  }

  alwaysinline auto operator[](uint index) const -> const T& {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(index >= Size) throw out_of_bounds{};
    #endif
    return values[index];
  }

  alwaysinline auto operator()(uint index, const T& fallback = {}) const -> const T& {
    if(index >= Size) return fallback;
    return values[index];
  }

  auto fill(const T& fill = {}) {
    for(auto& value : values) value = fill;
  }

  auto data() -> T* { return values; }
  auto data() const -> const T* { return values; }
  auto size() const -> uint { return Size; }

  auto begin() -> T* { return &values[0]; }
  auto end() -> T* { return &values[Size]; }

  auto begin() const -> const T* { return &values[0]; }
  auto end() const -> const T* { return &values[Size]; }

private:
  T values[Size];
};

template<typename T> struct array_view;

template<typename T, uint Size> struct array_view<T[Size]> {
  array_view() = default;

  template<uint Capacity>
  array_view(array<T[Capacity]>& source, uint offset = 0) {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(offset + Size >= Capacity) throw out_of_bounds{};
    #endif
    values = &source.data()[offset];
  }

  template<uint Capacity>
  array_view(T (&source)[Capacity], uint offset = 0) {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(offset + Size >= Capacity) throw out_of_bounds{};
    #endif
    values = &source[offset];
  }

  array_view(T* source, uint offset = 0) {
    values = &source[offset];
  }

  explicit operator bool() const {
    return values;
  }

  alwaysinline auto operator[](uint index) -> T& {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(index >= Size) throw out_of_bounds{};
    #endif
    return values[index];
  }

  alwaysinline auto operator[](uint index) const -> const T& {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(index >= Size) throw out_of_bounds{};
    #endif
    return values[index];
  }

  alwaysinline auto operator()(uint index, const T& fallback = {}) const -> const T& {
    if(index >= Size) return fallback;
    return values[index];
  }

  auto fill(const T& fill = {}) -> void {
    for(uint index : range(Size)) values[index] = fill;
  }

  auto data() -> T* { return values; }
  auto data() const -> const T* { return values; }
  auto size() const -> uint { return Size; }

  auto begin() -> T* { return &values[0]; }
  auto end() -> T* { return &values[Size]; }

  auto begin() const -> const T* { return &values[0]; }
  auto end() const -> const T* { return &values[Size]; }

private:
  T* values = nullptr;
};

}
