#ifndef NALL_BIT_HPP
#define NALL_BIT_HPP

#include <nall/type_traits.hpp>

namespace nall {
  template<unsigned bits, typename type_t = unsigned>
  constexpr inline type_t uclamp(const type_t x) {
    enum : type_t { b = 1ull << (bits - 1), y = b * 2 - 1 };
    return y + ((x - y) & -(x < y));  //min(x, y);
  }

  template<unsigned bits, typename type_t = unsigned>
  constexpr inline type_t uclip(const type_t x) {
    enum : type_t { b = 1ull << (bits - 1), m = b * 2 - 1 };
    return (x & m);
  }

  template<unsigned bits, typename type_t = signed>
  constexpr inline type_t sclamp(const type_t x) {
    enum : type_t { b = 1ull << (bits - 1), m = b - 1 };
    return (x > m) ? m : (x < -b) ? -b : x;
  }

  template<unsigned bits, typename type_t = signed>
  constexpr inline type_t sclip(const type_t x) {
    typedef typename type_if<std::is_same<type_t, signed>::value, unsigned, uintmax_t>::type cast_t;
    enum : cast_t { b = 1ull << (bits - 1), m = b * 2 - 1 };
    return ((x & m) ^ b) - b;
  }

  namespace bit {
    //lowest(0b1110) == 0b0010
    template<typename T>
    constexpr inline T lowest(const T x) {
      return x & -x;
    }

    //clear_lowest(0b1110) == 0b1100
    template<typename T>
    constexpr inline T clear_lowest(const T x) {
      return x & (x - 1);
    }

    //set_lowest(0b0101) == 0b0111
    template<typename T>
    constexpr inline T set_lowest(const T x) {
      return x | (x + 1);
    }

    //count number of bits set in a byte
    template<typename T>
    inline unsigned count(T x) {
      unsigned count = 0;
      do count += x & 1; while(x >>= 1);
      return count;
    }

    //round up to next highest single bit:
    //round(15) == 16, round(16) == 16, round(17) == 32
    template<typename T>
    inline T round(T x) {
      if((x & (x - 1)) == 0) return x;
      while(x & (x - 1)) x &= x - 1;
      return x << 1;
    }
  }
}

#endif
