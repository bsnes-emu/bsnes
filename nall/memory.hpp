#pragma once

#include <nall/algorithm.hpp>
#include <nall/stdint.hpp>

namespace nall { namespace memory {
  inline auto allocate(uint size) -> void*;
  inline auto allocate(uint size, uint8_t data) -> void*;

  inline auto resize(void* target, uint size) -> void*;

  inline auto free(void* target) -> void;

  inline auto compare(const void* target, uint capacity, const void* source, uint size) -> int;
  inline auto compare(const void* target, const void* source, uint size) -> int;

  inline auto icompare(const void* target, uint capacity, const void* source, uint size) -> int;
  inline auto icompare(const void* target, const void* source, uint size) -> int;

  inline auto copy(void* target, uint capacity, const void* source, uint size) -> void*;
  inline auto copy(void* target, const void* source, uint size) -> void*;

  inline auto move(void* target, uint capacity, const void* source, uint size) -> void*;
  inline auto move(void* target, const void* source, uint size) -> void*;

  inline auto fill(void* target, uint capacity, uint8_t data = 0x00) -> void*;

  template<typename T> inline auto assign(T* target) -> void {}
  template<typename T, typename U, typename... P> inline auto assign(T* target, const U& value, P&&... p) -> void;

  template<uint size, typename T = uint64_t> inline auto readl(const void* source) -> T;
  template<uint size, typename T = uint64_t> inline auto readm(const void* source) -> T;

  template<uint size, typename T = uint64_t> inline auto writel(void* target, T data) -> void;
  template<uint size, typename T = uint64_t> inline auto writem(void* target, T data) -> void;
}}

namespace nall {

//implementation notes:
//memcmp, memcpy, memmove have terrible performance on small block sizes (FreeBSD 10.0-amd64)
//as this library is used extensively by nall/string, and most strings tend to be small,
//this library hand-codes these functions instead. surprisingly, it's a substantial speedup

auto memory::allocate(uint size) -> void* {
  return malloc(size);
}

auto memory::allocate(uint size, uint8_t data) -> void* {
  auto result = malloc(size);
  if(result) fill(result, size, data);
  return result;
}

auto memory::resize(void* target, uint size) -> void* {
  return realloc(target, size);
}

auto memory::free(void* target) -> void {
  ::free(target);
}

auto memory::compare(const void* target, uint capacity, const void* source, uint size) -> int {
  auto t = (int8_t*)target;
  auto s = (int8_t*)source;
  auto l = min(capacity, size);
  while(l--) {
    auto x = *t++;
    auto y = *s++;
    if(x != y) return x - y;
  }
  return 0;
}

auto memory::compare(const void* target, const void* source, uint size) -> int {
  return compare(target, size, source, size);
}

auto memory::icompare(const void* target, uint capacity, const void* source, uint size) -> int {
  auto t = (int8_t*)target;
  auto s = (int8_t*)source;
  auto l = min(capacity, size);
  while(l--) {
    auto x = *t++;
    auto y = *s++;
    if(x - 'A' < 26) x += 32;
    if(y - 'A' < 26) y += 32;
    if(x != y) return x - y;
  }
  return 0;
}

auto memory::icompare(const void* target, const void* source, uint size) -> int {
  return icompare(target, size, source, size);
}

auto memory::copy(void* target, uint capacity, const void* source, uint size) -> void* {
  auto t = (uint8_t*)target;
  auto s = (uint8_t*)source;
  auto l = min(capacity, size);
  while(l--) *t++ = *s++;
  return target;
}

auto memory::copy(void* target, const void* source, uint size) -> void* {
  return copy(target, size, source, size);
}

auto memory::move(void* target, uint capacity, const void* source, uint size) -> void* {
  auto t = (uint8_t*)target;
  auto s = (uint8_t*)source;
  auto l = min(capacity, size);
  if(t < s) {
    while(l--) *t++ = *s++;
  } else {
    t += l;
    s += l;
    while(l--) *--t = *--s;
  }
  return target;
}

auto memory::move(void* target, const void* source, uint size) -> void* {
  return move(target, size, source, size);
}

auto memory::fill(void* target, uint capacity, uint8_t data) -> void* {
  auto t = (uint8_t*)target;
  while(capacity--) *t++ = data;
  return target;
}

template<typename T, typename U, typename... P>
auto memory::assign(T* target, const U& value, P&&... p) -> void {
  *target++ = value;
  assign(target, forward<P>(p)...);
}

template<uint size, typename T> auto memory::readl(const void* source) -> T {
  auto p = (const uint8_t*)source;
  T data = 0;
  for(uint n = 0; n < size; n++) data |= T(*p++) << n * 8;
  return data;
}

template<uint size, typename T> auto memory::readm(const void* source) -> T {
  auto p = (const uint8_t*)source;
  T data = 0;
  for(int n = size - 1; n >= 0; n--) data |= T(*p++) << n * 8;
  return data;
}

template<uint size, typename T> auto memory::writel(void* target, T data) -> void {
  auto p = (uint8_t*)target;
  for(uint n = 0; n < size; n++) *p++ = data >> n * 8;
}

template<uint size, typename T> auto memory::writem(void* target, T data) -> void {
  auto p = (uint8_t*)target;
  for(int n = size - 1; n >= 0; n--) *p++ = data >> n * 8;
}

}
