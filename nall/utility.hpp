#pragma once

#include <utility>

namespace nall {

template<typename T> struct base_from_member {
  base_from_member(T value) : value(value) {}
  T value;
};

template<typename T> inline auto allocate(unsigned size, const T& value) -> T* {
  T* array = new T[size];
  for(unsigned i = 0; i < size; i++) array[i] = value;
  return array;
}

}
