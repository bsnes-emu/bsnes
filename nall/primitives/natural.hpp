#pragma once

namespace nall {

template<int RequestedPrecision> struct Natural {
  enum : uint { Precision = RequestedPrecision < 1 ? 1 : RequestedPrecision > 64 ? 64 : RequestedPrecision };
  static inline constexpr auto bits() -> uint { return Precision; }
  using type =
    typename conditional<bits() <=  8,  uint8_t,
    typename conditional<bits() <= 16, uint16_t,
    typename conditional<bits() <= 32, uint32_t,
    typename conditional<bits() <= 64, uint64_t,
    void>::type>::type>::type>::type;
  static inline constexpr auto mask() -> type { return ~0ull >> 64 - bits(); }

  inline Natural() : data(0) {}
  template<typename T> inline Natural(const T& value) { data = mask(value); }

  explicit inline operator bool() const { return (bool)data; }
  inline operator type() const { return data; }

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

  inline auto serialize(serializer& s) { s(data); }

  inline auto zero() const -> bool { return data == 0; }
  inline auto positive() const -> bool { return data >> bits() - 1 == 0; }
  inline auto negative() const -> bool { return data >> bits() - 1 == 1; }

  inline auto bits(uint lo, uint hi) -> BitRange<RequestedPrecision> { return {(type&)data, lo, hi}; }
  inline auto bit(uint index) -> BitRange<RequestedPrecision> { return {(type&)data, index, index}; }
  inline auto byte(uint index) -> BitRange<RequestedPrecision> { return {(type&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(uint lo, uint hi) const -> const BitRange<RequestedPrecision> { return {(type&)data, lo, hi}; }
  inline auto bit(uint index) const -> const BitRange<RequestedPrecision> { return {(type&)data, index, index}; }
  inline auto byte(uint index) const -> const BitRange<RequestedPrecision> { return {(type&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> uintmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data < m ? data : m;
  }

  inline auto clip(uint bits) -> uintmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data & m;
  }

  inline auto integer() const -> Integer<RequestedPrecision>;

private:
  inline auto mask(type value) const -> type {
    return value & mask();
  }

  type data;
};

}
