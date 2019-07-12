#pragma once

struct BitsCaptureLo{};
struct BitsCaptureHi{};

struct BitsLo {
  const uint lo;
  inline BitsLo(uint lo) : lo(lo) {}
};

struct BitsHi {
  const uint hi;
  inline BitsHi(uint hi) : hi(hi) {}
};

struct BitsRange {
  const uint lo, hi;
  inline BitsRange(uint lo, uint hi) : lo(lo), hi(hi) {}
};

inline auto operator*(BitsCaptureLo, uint lhs) { return BitsLo{lhs}; }
inline auto operator*(uint rhs, BitsCaptureHi) { return BitsHi{rhs}; }
inline auto operator-(const BitsLo& lhs, const BitsHi& rhs) { return BitsRange{lhs.lo, rhs.hi}; }

template<typename T> struct Bits {
  T& value;
  using type =
    conditional_t<sizeof(T) <= 1,  uint8_t,
    conditional_t<sizeof(T) <= 2, uint16_t,
    conditional_t<sizeof(T) <= 4, uint32_t,
    conditional_t<sizeof(T) <= 8, uint64_t,
    void>>>>;
  const uint lo;
  const type mask;

  inline Bits(T& value, BitsRange range) : value(value), lo(range.lo), mask(~0ull >> 64 - (range.hi - range.lo + 1) << range.lo) {}
  inline Bits(const Bits& source) { operator=((uint64_t)source); }
  inline auto operator=(const Bits& source) -> Bits& { return operator=((uint64_t)source); }

  inline operator type() const {
    return (value & mask) >> lo;
  }

  template<typename U> inline auto operator=(U source) -> Bits& {
    value = value & ~mask | source << lo & mask;
    return *this;
  }

  template<typename U> inline auto operator&=(U source) -> Bits& {
    value = value & (~mask | source << lo & mask);
    return *this;
  }

  template<typename U> inline auto operator^=(U source) -> Bits& {
    value = value ^ source << lo & mask;
    return *this;
  }

  template<typename U> inline auto operator|=(U source) -> Bits& {
    value = value | source << lo & mask;
    return *this;
  }
};

template<typename T> struct Bits<const T> {
  T value;
  using type =
    conditional_t<sizeof(T) <= 1,  uint8_t,
    conditional_t<sizeof(T) <= 2, uint16_t,
    conditional_t<sizeof(T) <= 4, uint32_t,
    conditional_t<sizeof(T) <= 8, uint64_t,
    void>>>>;
  const uint lo;
  const type mask;

  inline Bits(const T& value, BitsRange range) : value(value), lo(range.lo), mask(~0ull >> 64 - (range.hi - range.lo + 1) << range.lo) {}
  Bits(const Bits& source) = delete;
  auto operator=(const Bits& source) -> Bits& = delete;

  inline operator type() const {
    return (value & mask) >> lo;
  }
};

#define bits(value,range) Bits<decltype(value)>{value,BitsCaptureLo{}*range*BitsCaptureHi{}}
#define bit1(value,index) Bits<decltype(value)>{value,BitsRange{(uint)index,(uint)index}}
#define bit8(value,index) Bits<decltype(value)>{value,BitsRange{(uint)index*8,(uint)index*8+7}}

#define cbits(value,range) Bits<const decltype(value)>{value,BitsCaptureLo{}*range*BitsCaptureHi{}}
#define cbit1(value,index) Bits<const decltype(value)>{value,BitsRange{(uint)index,(uint)index}}
#define cbit8(value,index) Bits<const decltype(value)>{value,BitsRange{(uint)index*8,(uint)index*8+7}}
