#ifndef NALL_NEW_HPP
#define NALL_NEW_HPP

#include <string.h>
#include <new>
#include <nall/stdint.hpp>

namespace nall {
  struct zeromemory_t {};
  static zeromemory_t zeromemory;
}

inline void* operator new[](size_t size, const nall::zeromemory_t&) throw(std::bad_alloc) {
  void *p = new uint8_t[size];
  memset(p, 0, size);
  return p;
}

inline void* operator new[](size_t size, const std::nothrow_t&, const nall::zeromemory_t&) throw() {
  void *p = new(std::nothrow) uint8_t[size];
  if(p) memset(p, 0, size);
  return p;
}

#endif
