#pragma once

namespace nall {

template<uint Precision = 64> struct Natural {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using utype =
    conditional_t<bits() <=  8,  uint8_t,
    conditional_t<bits() <= 16, uint16_t,
    conditional_t<bits() <= 32, uint32_t,
    conditional_t<bits() <= 64, uint64_t,
    void>>>>;
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

  inline auto operator!() const { return !data; }
  inline auto operator~() const { return Natural<>{mask(~data)}; }
  inline auto operator+() const { return Natural<>{+data}; }
  inline auto operator-() const { return Natural<>{-(uint64_t)data}; }

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

  //warning: this does not and cannot short-circuit; value is always evaluated
  template<typename T> inline auto orElse(const T& value) { return Natural<>{data ? data : value}; }

  inline auto bits(int lo, int hi) -> BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) const -> const BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) const -> const BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto slice(int index) const { return Natural<>{bit(index)}; }
  inline auto slice(int lo, int hi) const { return Natural<>{bits(lo, hi)}; }

  inline auto clamp(uint bits) {
    const uint64_t b = 1ull << (bits - 1);
    const uint64_t m = b * 2 - 1;
    return Natural<>{data < m ? data : m};
  }

  inline auto clip(uint bits) {
    const uint64_t b = 1ull << (bits - 1);
    const uint64_t m = b * 2 - 1;
    return Natural<>{data & m};
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
#define rhs r
template<int LHS, int RHS> inline auto operator *(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  * rhs}; }
template<int LHS, int RHS> inline auto operator /(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  / rhs}; }
template<int LHS, int RHS> inline auto operator %(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  % rhs}; }
template<int LHS, int RHS> inline auto operator +(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  + rhs}; }
template<int LHS, int RHS> inline auto operator -(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  - rhs}; }
template<int LHS, int RHS> inline auto operator<<(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs << rhs}; }
template<int LHS, int RHS> inline auto operator>>(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs >> rhs}; }
template<int LHS, int RHS> inline auto operator &(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  & rhs}; }
template<int LHS, int RHS> inline auto operator ^(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  ^ rhs}; }
template<int LHS, int RHS> inline auto operator |(Natural<LHS> l, Natural<RHS> r) { return Natural{lhs  | rhs}; }
#undef lhs
#undef rhs

}
