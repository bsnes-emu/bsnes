#pragma once

namespace nall {

template<int Precision> struct Natural {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using utype =
    typename conditional<bits() <=  8,  uint8_t,
    typename conditional<bits() <= 16, uint16_t,
    typename conditional<bits() <= 32, uint32_t,
    typename conditional<bits() <= 64, uint64_t,
    void>::type>::type>::type>::type;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }

  inline Natural() : data(0) {}
  template<int Bits> inline Natural(Natural<Bits> value) { data = mask(value); }
  template<typename T> inline Natural(const T& value) { data = mask(value); }

  explicit inline operator bool() const { return data; }
  inline operator uint64_t() const { return data; }

  inline auto operator++(int) { auto value = *this; data = mask(data + 1); return value; }
  inline auto operator--(int) { auto value = *this; data = mask(data - 1); return value; }

  inline auto& operator++() { data = mask(data + 1); return *this; }
  inline auto& operator--() { data = mask(data - 1); return *this; }

  inline auto operator!() const { return Natural{!data}; }
  inline auto operator~() const { return Natural{~data}; }
  inline auto operator+() const { return Natural{+data}; }
  inline auto operator-() const { return Natural{-data}; }

  #define lhs data
  #define rhs value
  template<typename T> inline auto& operator  =(const T& value) { lhs = mask(       rhs); return *this; }
  template<typename T> inline auto& operator *=(const T& value) { lhs = mask(lhs  * rhs); return *this; }
  template<typename T> inline auto& operator /=(const T& value) { lhs = mask(lhs  / rhs); return *this; }
  template<typename T> inline auto& operator %=(const T& value) { lhs = mask(lhs  % rhs); return *this; }
  template<typename T> inline auto& operator +=(const T& value) { lhs = mask(lhs  + rhs); return *this; }
  template<typename T> inline auto& operator -=(const T& value) { lhs = mask(lhs  - rhs); return *this; }
  template<typename T> inline auto& operator<<=(const T& value) { lhs = mask(lhs << rhs); return *this; }
  template<typename T> inline auto& operator>>=(const T& value) { lhs = mask(lhs >> rhs); return *this; }
  template<typename T> inline auto& operator &=(const T& value) { lhs = mask(lhs  & rhs); return *this; }
  template<typename T> inline auto& operator ^=(const T& value) { lhs = mask(lhs  ^ rhs); return *this; }
  template<typename T> inline auto& operator |=(const T& value) { lhs = mask(lhs  | rhs); return *this; }
  #undef lhs
  #undef rhs

  //work in progress: we want natural<op>primitive and primitive<op>natural to result in natural<64> ...
  //however, these operators will create ambiguous overloads unless operator uint64_t() is explicit.
  //a large volume of existing code will need to be updated before this will be possible.
  #if 0
  #define lhs (uint64_t)data
  #define rhs value
  template<typename T> inline auto operator *(const T& value) { return Natural<64>{lhs  * rhs}; }
  template<typename T> inline auto operator /(const T& value) { return Natural<64>{lhs  / rhs}; }
  template<typename T> inline auto operator %(const T& value) { return Natural<64>{lhs  % rhs}; }
  template<typename T> inline auto operator +(const T& value) { return Natural<64>{lhs  + rhs}; }
  template<typename T> inline auto operator -(const T& value) { return Natural<64>{lhs  - rhs}; }
  template<typename T> inline auto operator<<(const T& value) { return Natural<64>{lhs << rhs}; }
  template<typename T> inline auto operator>>(const T& value) { return Natural<64>{lhs >> rhs}; }
  template<typename T> inline auto operator &(const T& value) { return Natural<64>{lhs  & rhs}; }
  template<typename T> inline auto operator ^(const T& value) { return Natural<64>{lhs  ^ rhs}; }
  template<typename T> inline auto operator |(const T& value) { return Natural<64>{lhs  | rhs}; }
  #undef lhs
  #undef rhs

  #define lhs l
  #define rhs (uint64_t)r
  template<typename T> friend inline auto operator *(const T& l, Natural r) { return Natural<64>{lhs  * rhs}; }
  template<typename T> friend inline auto operator %(const T& l, Natural r) { return Natural<64>{lhs  / rhs}; }
  template<typename T> friend inline auto operator /(const T& l, Natural r) { return Natural<64>{lhs  % rhs}; }
  template<typename T> friend inline auto operator +(const T& l, Natural r) { return Natural<64>{lhs  + rhs}; }
  template<typename T> friend inline auto operator -(const T& l, Natural r) { return Natural<64>{lhs  - rhs}; }
  template<typename T> friend inline auto operator<<(const T& l, Natural r) { return Natural<64>{lhs << rhs}; }
  template<typename T> friend inline auto operator>>(const T& l, Natural r) { return Natural<64>{lhs >> rhs}; }
  template<typename T> friend inline auto operator &(const T& l, Natural r) { return Natural<64>{lhs  & rhs}; }
  template<typename T> friend inline auto operator ^(const T& l, Natural r) { return Natural<64>{lhs  ^ rhs}; }
  template<typename T> friend inline auto operator |(const T& l, Natural r) { return Natural<64>{lhs  | rhs}; }
  #undef lhs
  #undef rhs
  #endif

  inline auto bits(int lo, int hi) -> BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) const -> const BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) const -> const BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return Natural<64>{data < m ? data : m};
  }

  inline auto clip(uint bits) {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return Natural<64>{data & m};
  }

  inline auto serialize(serializer& s) { s(data); }
  inline auto integer() const -> Integer<Precision>;

private:
  inline auto mask(utype value) const -> utype {
    return value & mask();
  }

  utype data;
};

#define lhs (uint64_t)l
#define rhs (uint64_t)r
template<int LHS, int RHS> inline auto operator *(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  * rhs}; }
template<int LHS, int RHS> inline auto operator /(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  / rhs}; }
template<int LHS, int RHS> inline auto operator %(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  % rhs}; }
template<int LHS, int RHS> inline auto operator +(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  + rhs}; }
template<int LHS, int RHS> inline auto operator -(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  - rhs}; }
template<int LHS, int RHS> inline auto operator<<(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs << rhs}; }
template<int LHS, int RHS> inline auto operator>>(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs >> rhs}; }
template<int LHS, int RHS> inline auto operator &(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  & rhs}; }
template<int LHS, int RHS> inline auto operator ^(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  ^ rhs}; }
template<int LHS, int RHS> inline auto operator |(Natural<LHS> l, Natural<RHS> r) { return Natural<64>{lhs  | rhs}; }
#undef lhs
#undef rhs

}
