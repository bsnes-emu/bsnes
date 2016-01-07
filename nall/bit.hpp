#pragma once

#include <nall/stdint.hpp>

namespace nall {

template<unsigned bits> inline auto uclamp(const uintmax_t x) -> uintmax_t {
  enum : uintmax_t { b = 1ull << (bits - 1), y = b * 2 - 1 };
  return y + ((x - y) & -(x < y));  //min(x, y);
}

template<unsigned bits> inline auto uclip(const uintmax_t x) -> uintmax_t {
  enum : uintmax_t { b = 1ull << (bits - 1), m = b * 2 - 1 };
  return (x & m);
}

template<unsigned bits> inline auto sclamp(const intmax_t x) -> intmax_t {
  enum : intmax_t { b = 1ull << (bits - 1), m = b - 1 };
  return (x > m) ? m : (x < -b) ? -b : x;
}

template<unsigned bits> inline auto sclip(const intmax_t x) -> intmax_t {
  enum : uintmax_t { b = 1ull << (bits - 1), m = b * 2 - 1 };
  return ((x & m) ^ b) - b;
}

namespace bit {
  constexpr inline auto mask(const char* s, uintmax_t sum = 0) -> uintmax_t {
    return (
      *s == '0' || *s == '1' ? mask(s + 1, (sum << 1) | 1) :
      *s == ' ' || *s == '_' ? mask(s + 1, sum) :
      *s ? mask(s + 1, sum << 1) :
      sum
    );
  }

  constexpr inline auto test(const char* s, uintmax_t sum = 0) -> uintmax_t {
    return (
      *s == '0' || *s == '1' ? test(s + 1, (sum << 1) | (*s - '0')) :
      *s == ' ' || *s == '_' ? test(s + 1, sum) :
      *s ? test(s + 1, sum << 1) :
      sum
    );
  }

  //lowest(0b1110) == 0b0010
  constexpr inline auto lowest(const uintmax_t x) -> uintmax_t {
    return x & -x;
  }

  //clear_lowest(0b1110) == 0b1100
  constexpr inline auto clear_lowest(const uintmax_t x) -> uintmax_t {
    return x & (x - 1);
  }

  //set_lowest(0b0101) == 0b0111
  constexpr inline auto set_lowest(const uintmax_t x) -> uintmax_t {
    return x | (x + 1);
  }

  //count number of bits set in a byte
  inline auto count(uintmax_t x) -> unsigned {
    unsigned count = 0;
    do count += x & 1; while(x >>= 1);
    return count;
  }

  //return index of the first bit set (or zero of no bits are set)
  //first(0b1000) == 3
  inline auto first(uintmax_t x) -> unsigned {
    unsigned first = 0;
    while(x) { if(x & 1) break; x >>= 1; first++; }
    return first;
  }

  //round up to next highest single bit:
  //round(15) == 16, round(16) == 16, round(17) == 32
  inline auto round(uintmax_t x) -> uintmax_t {
    if((x & (x - 1)) == 0) return x;
    while(x & (x - 1)) x &= x - 1;
    return x << 1;
  }
}

}
