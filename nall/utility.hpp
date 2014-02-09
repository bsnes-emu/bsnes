#ifndef NALL_UTILITY_HPP
#define NALL_UTILITY_HPP

#include <utility>

namespace nall {

template<typename T> struct base_from_member {
  T value;
  base_from_member(T value) : value(value) {}
};

template<typename T> inline T* allocate(unsigned size, const T& value) {
  T* array = new T[size];
  for(unsigned i = 0; i < size; i++) array[i] = value;
  return array;
}

}

#endif
