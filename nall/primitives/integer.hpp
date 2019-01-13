#pragma once

namespace nall {

template<int RequestedPrecision> struct Integer {
  enum : uint { Precision = RequestedPrecision < 1 ? 1 : RequestedPrecision > 64 ? 64 : RequestedPrecision };
  static inline constexpr auto bits() -> uint { return Precision; }
  using type =
    typename conditional<bits() <=  8,  int8_t,
    typename conditional<bits() <= 16, int16_t,
    typename conditional<bits() <= 32, int32_t,
    typename conditional<bits() <= 64, int64_t,
    void>::type>::type>::type>::type;
  using utype = typename Natural<RequestedPrecision>::type;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }
  static inline constexpr auto sign() -> utype { return 1ull << Precision - 1; }

  inline Integer() : data(0) {}
  template<typename T> inline Integer(const T& value) { data = mask(value); }

  explicit inline operator bool() const { return (bool)data; }
  inline operator type() const { return data; }

  inline auto operator++(int) { auto value = *this; data = mask(data + 1); return value; }
  inline auto operator--(int) { auto value = *this; data = mask(data - 1); return value; }

  inline auto& operator++() { data = mask(data + 1); return *this; }
  inline auto& operator--() { data = mask(data - 1); return *this; }

  inline auto operator!() const { return Integer{!data}; }
  inline auto operator~() const { return Integer{~data}; }
  inline auto operator+() const { return Integer{+data}; }
  inline auto operator-() const { return Integer{-data}; }

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
  #undef rfs

  inline auto serialize(serializer& s) { s(data); }

  inline auto zero() const -> bool { return data == 0; }
  inline auto positive() const -> bool { return data >= 0; }
  inline auto negative() const -> bool { return data <  0; }

  inline auto bits(uint lo, uint hi) -> BitRange<RequestedPrecision> { return {(utype&)data, lo, hi}; }
  inline auto bit(uint index) -> BitRange<RequestedPrecision> { return {(utype&)data, index, index}; }
  inline auto byte(uint index) -> BitRange<RequestedPrecision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(uint lo, uint hi) const -> const BitRange<RequestedPrecision> { return {(utype&)*this, lo, lo}; }
  inline auto bit(uint index) const -> const BitRange<RequestedPrecision> { return {(utype&)*this, index, index}; }
  inline auto byte(uint index) const -> const BitRange<RequestedPrecision> { return {(utype&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> intmax {
    const intmax b = 1ull << (bits - 1);
    const intmax m = b - 1;
    return data > m ? m : data < -b ? -b : data;
  }

  inline auto clip(uint bits) -> intmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return ((data & m) ^ b) - b;
  }

  inline auto natural() const -> Natural<RequestedPrecision>;

private:
  auto mask(type value) const -> type {
    return (value & mask() ^ sign()) - sign();
  }

  type data;
};

}
