#pragma once

namespace nall {

template<int Requested> struct Integer {
  enum : uint { Precision = Requested < 1 ? 1 : Requested > 64 ? 64 : Requested };
  static inline constexpr auto bits() -> uint { return Precision; }
  using type =
    typename conditional<bits() <=  8,  int8_t,
    typename conditional<bits() <= 16, int16_t,
    typename conditional<bits() <= 32, int32_t,
    typename conditional<bits() <= 64, int64_t,
    void>::type>::type>::type>::type;
  using utype = typename Natural<Requested>::type;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }
  static inline constexpr auto sign() -> utype { return 1ull << Precision - 1; }

  inline Integer() : data(0) {}
  template<int Bits> inline Integer(Integer<Bits> value) { data = mask(value); }
  template<typename T> inline Integer(const T& value) { data = mask(value); }

  explicit inline operator bool() const { return data; }
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
  #undef rhs

  inline auto bits(int lo, int hi) -> BitRange<Requested> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Requested> { return {(utype&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Requested> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Requested> { return {(utype&)*this, lo, lo}; }
  inline auto bit(int index) const -> const BitRange<Requested> { return {(utype&)*this, index, index}; }
  inline auto byte(int index) const -> const BitRange<Requested> { return {(utype&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> type {
    const intmax b = 1ull << (bits - 1);
    const intmax m = b - 1;
    return data > m ? m : data < -b ? -b : data;
  }

  inline auto clip(uint bits) -> type {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return ((data & m) ^ b) - b;
  }

  inline auto serialize(serializer& s) { s(data); }
  inline auto natural() const -> Natural<Requested>;

private:
  inline auto mask(type value) const -> type {
    return (value & mask() ^ sign()) - sign();
  }

  type data;
};

#define ALL 64
#define ADD 64  //LHS + RHS
#define INC 64  //1 + (LHS >= RHS ? LHS : RHS)
#define MAX 64  //LHS >= RHS ? LHS : RHS
#define MIN 64  //LHS <= RHS ? LHS : RHS
#define lhs (int64_t)(typename Integer<LHS>::type)l
#define rhs (typename Integer<RHS>::type)r
template<int LHS, int RHS> inline auto operator *(Integer<LHS> l, Integer<RHS> r) { return Integer<ADD>{lhs  * rhs}; }
template<int LHS, int RHS> inline auto operator /(Integer<LHS> l, Integer<RHS> r) { return Integer<LHS>{lhs  / rhs}; }
template<int LHS, int RHS> inline auto operator %(Integer<LHS> l, Integer<RHS> r) { return Integer<LHS>{lhs  % rhs}; }
template<int LHS, int RHS> inline auto operator +(Integer<LHS> l, Integer<RHS> r) { return Integer<INC>{lhs  + rhs}; }
template<int LHS, int RHS> inline auto operator -(Integer<LHS> l, Integer<RHS> r) { return Integer<INC>{lhs  - rhs}; }
template<int LHS, int RHS> inline auto operator<<(Integer<LHS> l, Integer<RHS> r) { return Integer<ALL>{lhs << rhs}; }
template<int LHS, int RHS> inline auto operator>>(Integer<LHS> l, Integer<RHS> r) { return Integer<LHS>{lhs >> rhs}; }
template<int LHS, int RHS> inline auto operator &(Integer<LHS> l, Integer<RHS> r) { return Integer<MAX>{lhs  & rhs}; }
template<int LHS, int RHS> inline auto operator ^(Integer<LHS> l, Integer<RHS> r) { return Integer<MAX>{lhs  ^ rhs}; }
template<int LHS, int RHS> inline auto operator |(Integer<LHS> l, Integer<RHS> r) { return Integer<MAX>{lhs  | rhs}; }
#undef ALL
#undef ADD
#undef INC
#undef MAX
#undef MIN
#undef lhs
#undef rhs

}
