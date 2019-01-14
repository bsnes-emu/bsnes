#pragma once

namespace nall {

template<int Requested> struct Natural {
  enum : uint { Precision = Requested < 1 ? 1 : Requested > 64 ? 64 : Requested };
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

  inline auto bits(int lo, int hi) -> BitRange<Requested> { return {(type&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Requested> { return {(type&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Requested> { return {(type&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Requested> { return {(type&)data, lo, hi}; }
  inline auto bit(int index) const -> const BitRange<Requested> { return {(type&)data, index, index}; }
  inline auto byte(int index) const -> const BitRange<Requested> { return {(type&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> type {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data < m ? data : m;
  }

  inline auto clip(uint bits) -> type {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data & m;
  }

  inline auto serialize(serializer& s) { s(data); }
  inline auto integer() const -> Integer<Requested>;

private:
  inline auto mask(type value) const -> type {
    return value & mask();
  }

  type data;
};

}
