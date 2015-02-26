#ifdef NALL_MEMORY_INTERNAL_HPP

#include <nall/algorithm.hpp>

namespace nall {

namespace memory {
  inline auto allocate(unsigned size) -> void*;
  inline auto allocate(unsigned size, uint8_t data) -> void*;

  inline auto resize(void* target, unsigned size) -> void*;

  inline auto free(void* target) -> void;

  inline auto compare(const void* target, unsigned capacity, const void* source, unsigned size) -> signed;
  inline auto compare(const void* target, const void* source, unsigned size) -> signed;

  inline auto icompare(const void* target, unsigned capacity, const void* source, unsigned size) -> signed;
  inline auto icompare(const void* target, const void* source, unsigned size) -> signed;

  inline auto copy(void* target, unsigned capacity, const void* source, unsigned size) -> void*;
  inline auto copy(void* target, const void* source, unsigned size) -> void*;

  inline auto move(void* target, unsigned capacity, const void* source, unsigned size) -> void*;
  inline auto move(void* target, const void* source, unsigned size) -> void*;

  inline auto fill(void* target, unsigned capacity, uint8_t data = 0x00) -> void*;
}

}

#include <nall/memory/pool.hpp>

namespace nall {

//implementation notes:
//memcmp, memcpy, memmove have terrible performance on small block sizes (FreeBSD 10.0-amd64)
//as this library is used extensively by nall/string, and most strings tend to be small,
//this library hand-codes these functions instead. surprisingly, it's a substantial speedup

auto memory::allocate(unsigned size) -> void* {
  return malloc(size);
}

auto memory::allocate(unsigned size, uint8_t data) -> void* {
  auto result = malloc(size);
  if(result) fill(result, size, data);
  return result;
}

auto memory::resize(void* target, unsigned size) -> void* {
  return realloc(target, size);
}

auto memory::free(void* target) -> void {
  ::free(target);
}

auto memory::compare(const void* target, unsigned capacity, const void* source, unsigned size) -> signed {
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

auto memory::compare(const void* target, const void* source, unsigned size) -> signed {
  return compare(target, size, source, size);
}

auto memory::icompare(const void* target, unsigned capacity, const void* source, unsigned size) -> signed {
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

auto memory::icompare(const void* target, const void* source, unsigned size) -> signed {
  return icompare(target, size, source, size);
}

auto memory::copy(void* target, unsigned capacity, const void* source, unsigned size) -> void* {
  auto t = (uint8_t*)target;
  auto s = (uint8_t*)source;
  auto l = min(capacity, size);
  while(l--) *t++ = *s++;
  return target;
}

auto memory::copy(void* target, const void* source, unsigned size) -> void* {
  return copy(target, size, source, size);
}

auto memory::move(void* target, unsigned capacity, const void* source, unsigned size) -> void* {
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

auto memory::move(void* target, const void* source, unsigned size) -> void* {
  return move(target, size, source, size);
}

auto memory::fill(void* target, unsigned capacity, uint8_t data) -> void* {
  auto t = (uint8_t*)target;
  while(capacity--) *t++ = data;
  return target;
}

}

#endif
