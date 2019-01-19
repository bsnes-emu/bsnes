#pragma once

namespace nall {

template<uint Precision = 64> struct Integer {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using stype =
    conditional_t<bits() <=  8,  int8_t,
    conditional_t<bits() <= 16, int16_t,
    conditional_t<bits() <= 32, int32_t,
    conditional_t<bits() <= 64, int64_t,
    void>>>>;
  using utype = typename Natural<Precision>::utype;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }
  static inline constexpr auto sign() -> utype { return 1ull << Precision - 1; }

  inline Integer() : data(0) {}
  template<int Bits> inline Integer(Integer<Bits> value) { data = mask(value); }
  template<typename T> inline Integer(const T& value) { data = mask(value); }

  explicit inline operator bool() const { return data; }
  inline operator int64_t() const { return data; }

  inline auto operator++(int) { auto value = *this; data = mask(data + 1); return value; }
  inline auto operator--(int) { auto value = *this; data = mask(data - 1); return value; }

  inline auto& operator++() { data = mask(data + 1); return *this; }
  inline auto& operator--() { data = mask(data - 1); return *this; }

  inline auto operator!() const { return Integer{!data}; }
  inline auto operator~() const { return Integer{~data}; }
  inline auto operator+() const { return Integer<>{+(int64_t)data}; }
  inline auto operator-() const { return Integer<>{-(int64_t)data}; }

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

  #define lhs (int64_t)data
  #define rhs value
  #undef lhs
  #undef rhs

  //warning: this does not and cannot short-circuit; value is always evaluated
  template<typename T> inline auto orElse(const T& value) { return Integer<>{data ? data : value}; }

  inline auto bits(int lo, int hi) -> BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Precision> { return {(utype&)*this, lo, lo}; }
  inline auto bit(int index) const -> const BitRange<Precision> { return {(utype&)*this, index, index}; }
  inline auto byte(int index) const -> const BitRange<Precision> { return {(utype&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto slice(int index) const { return Natural<>{bit(index)}; }
  inline auto slice(int lo, int hi) const { return Natural<>{bit(lo, hi)}; }

  inline auto clamp(uint bits) {
    const int64_t b = 1ull << (bits - 1);
    const int64_t m = b - 1;
    return Integer<>{data > m ? m : data < -b ? -b : data};
  }

  inline auto clip(uint bits) {
    const uint64_t b = 1ull << (bits - 1);
    const uint64_t m = b * 2 - 1;
    return Integer<>{(data & m ^ b) - b};
  }

  inline auto serialize(serializer& s) { s(data); }
  inline auto natural() const -> Natural<Precision>;

private:
  inline auto mask(stype value) const -> stype {
    return (value & mask() ^ sign()) - sign();
  }

  stype data;
};

#define lhs (int64_t)l
#define rhs r
template<int LHS, int RHS> inline auto operator *(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  * rhs}; }
template<int LHS, int RHS> inline auto operator /(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  / rhs}; }
template<int LHS, int RHS> inline auto operator %(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  % rhs}; }
template<int LHS, int RHS> inline auto operator +(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  + rhs}; }
template<int LHS, int RHS> inline auto operator -(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  - rhs}; }
template<int LHS, int RHS> inline auto operator<<(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs << rhs}; }
template<int LHS, int RHS> inline auto operator>>(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs >> rhs}; }
template<int LHS, int RHS> inline auto operator &(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  & rhs}; }
template<int LHS, int RHS> inline auto operator ^(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  ^ rhs}; }
template<int LHS, int RHS> inline auto operator |(Integer<LHS> l, Integer<RHS> r) { return Integer{lhs  | rhs}; }
#undef lhs
#undef rhs

}
