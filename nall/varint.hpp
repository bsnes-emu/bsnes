#ifndef NALL_VARINT_HPP
#define NALL_VARINT_HPP

#include <nall/bit.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/traits.hpp>

namespace nall {

struct varint {
  virtual auto read() -> uint8_t = 0;
  virtual auto write(uint8_t) -> void = 0;

  auto readvu() -> uintmax_t {
    uintmax_t data = 0, shift = 1;
    while(true) {
      uint8_t x = read();
      data += (x & 0x7f) * shift;
      if(x & 0x80) break;
      shift <<= 7;
      data += shift;
    }
    return data;
  }

  auto readvs() -> intmax_t {
    uintmax_t data = readvu();
    bool negate = data & 1;
    data >>= 1;
    if(negate) data = ~data;
    return data;
  }

  auto writevu(uintmax_t data) -> void {
    while(true) {
      uint8_t x = data & 0x7f;
      data >>= 7;
      if(data == 0) return write(0x80 | x);
      write(x);
      data--;
    }
  }

  auto writevs(intmax_t data) -> void {
    bool negate = data < 0;
    if(negate) data = ~data;
    data = (data << 1) | negate;
    writevu(data);
  }
};

template<unsigned bits> struct uint_t {
  using type_t = type_if<expression<bits <= 8 * sizeof(unsigned)>, unsigned, uintmax_t>;

  inline operator type_t() const { return data; }
  inline auto operator ++(int) { type_t r = data; data = uclip<bits>(data + 1); return r; }
  inline auto operator --(int) { type_t r = data; data = uclip<bits>(data - 1); return r; }
  inline auto operator ++() { return data = uclip<bits>(data + 1); }
  inline auto operator --() { return data = uclip<bits>(data - 1); }
  inline auto operator  =(const type_t i) { return data = uclip<bits>(i); }
  inline auto operator |=(const type_t i) { return data = uclip<bits>(data  | i); }
  inline auto operator ^=(const type_t i) { return data = uclip<bits>(data  ^ i); }
  inline auto operator &=(const type_t i) { return data = uclip<bits>(data  & i); }
  inline auto operator<<=(const type_t i) { return data = uclip<bits>(data << i); }
  inline auto operator>>=(const type_t i) { return data = uclip<bits>(data >> i); }
  inline auto operator +=(const type_t i) { return data = uclip<bits>(data  + i); }
  inline auto operator -=(const type_t i) { return data = uclip<bits>(data  - i); }
  inline auto operator *=(const type_t i) { return data = uclip<bits>(data  * i); }
  inline auto operator /=(const type_t i) { return data = uclip<bits>(data  / i); }
  inline auto operator %=(const type_t i) { return data = uclip<bits>(data  % i); }

  inline uint_t() : data(0) {}
  inline uint_t(const type_t i) : data(uclip<bits>(i)) {}

  template<unsigned s> inline uint_t(const uint_t<s>& i) : data(uclip<bits>(i)) {}
  template<unsigned s> inline auto operator=(const uint_t<s>& i) { return data = uclip<bits>((type_t)i); }

  auto serialize(serializer& s) { s(data); }

private:
  type_t data;
};

template<unsigned bits> struct int_t {
  using type_t = type_if<expression<bits <= 8 * sizeof(signed)>, signed, intmax_t>;

  inline operator type_t() const { return data; }
  inline auto operator ++(int) { type_t r = data; data = sclip<bits>(data + 1); return r; }
  inline auto operator --(int) { type_t r = data; data = sclip<bits>(data - 1); return r; }
  inline auto operator ++() { return data = sclip<bits>(data + 1); }
  inline auto operator --() { return data = sclip<bits>(data - 1); }
  inline auto operator  =(const type_t i) { return data = sclip<bits>(i); }
  inline auto operator |=(const type_t i) { return data = sclip<bits>(data  | i); }
  inline auto operator ^=(const type_t i) { return data = sclip<bits>(data  ^ i); }
  inline auto operator &=(const type_t i) { return data = sclip<bits>(data  & i); }
  inline auto operator<<=(const type_t i) { return data = sclip<bits>(data << i); }
  inline auto operator>>=(const type_t i) { return data = sclip<bits>(data >> i); }
  inline auto operator +=(const type_t i) { return data = sclip<bits>(data  + i); }
  inline auto operator -=(const type_t i) { return data = sclip<bits>(data  - i); }
  inline auto operator *=(const type_t i) { return data = sclip<bits>(data  * i); }
  inline auto operator /=(const type_t i) { return data = sclip<bits>(data  / i); }
  inline auto operator %=(const type_t i) { return data = sclip<bits>(data  % i); }

  inline int_t() : data(0) {}
  inline int_t(const type_t i) : data(sclip<bits>(i)) {}

  template<unsigned s> inline int_t(const int_t<s>& i) : data(sclip<bits>(i)) {}
  template<unsigned s> inline auto operator=(const int_t<s>& i) { return data = sclip<bits>((type_t)i); }

  auto serialize(serializer& s) { s(data); }

private:
  type_t data;
};

template<typename type_t> struct varuint_t {
  inline operator type_t() const { return data; }
  inline auto operator ++(int) { type_t r = data; data = (data + 1) & mask; return r; }
  inline auto operator --(int) { type_t r = data; data = (data - 1) & mask; return r; }
  inline auto operator ++() { return data = (data + 1) & mask; }
  inline auto operator --() { return data = (data - 1) & mask; }
  inline auto operator  =(const type_t i) { return data = (i) & mask; }
  inline auto operator |=(const type_t i) { return data = (data  | i) & mask; }
  inline auto operator ^=(const type_t i) { return data = (data  ^ i) & mask; }
  inline auto operator &=(const type_t i) { return data = (data  & i) & mask; }
  inline auto operator<<=(const type_t i) { return data = (data << i) & mask; }
  inline auto operator>>=(const type_t i) { return data = (data >> i) & mask; }
  inline auto operator +=(const type_t i) { return data = (data  + i) & mask; }
  inline auto operator -=(const type_t i) { return data = (data  - i) & mask; }
  inline auto operator *=(const type_t i) { return data = (data  * i) & mask; }
  inline auto operator /=(const type_t i) { return data = (data  / i) & mask; }
  inline auto operator %=(const type_t i) { return data = (data  % i) & mask; }

  inline auto bits(type_t bits) { mask = (1ull << (bits - 1)) + ((1ull << (bits - 1)) - 1); data &= mask; }
  inline varuint_t() : data(0ull), mask((type_t)~0ull) {}
  inline varuint_t(const type_t i) : data(i), mask((type_t)~0ull) {}

  auto serialize(serializer& s) { s(data); s(mask); }

private:
  type_t data;
  type_t mask;
};

}

//typedefs
  typedef nall::uint_t< 1>  uint1_t;
  typedef nall::uint_t< 2>  uint2_t;
  typedef nall::uint_t< 3>  uint3_t;
  typedef nall::uint_t< 4>  uint4_t;
  typedef nall::uint_t< 5>  uint5_t;
  typedef nall::uint_t< 6>  uint6_t;
  typedef nall::uint_t< 7>  uint7_t;
//typedef nall::uint_t< 8>  uint8_t;

  typedef nall::uint_t< 9>  uint9_t;
  typedef nall::uint_t<10> uint10_t;
  typedef nall::uint_t<11> uint11_t;
  typedef nall::uint_t<12> uint12_t;
  typedef nall::uint_t<13> uint13_t;
  typedef nall::uint_t<14> uint14_t;
  typedef nall::uint_t<15> uint15_t;
//typedef nall::uint_t<16> uint16_t;

  typedef nall::uint_t<17> uint17_t;
  typedef nall::uint_t<18> uint18_t;
  typedef nall::uint_t<19> uint19_t;
  typedef nall::uint_t<20> uint20_t;
  typedef nall::uint_t<21> uint21_t;
  typedef nall::uint_t<22> uint22_t;
  typedef nall::uint_t<23> uint23_t;
  typedef nall::uint_t<24> uint24_t;
  typedef nall::uint_t<25> uint25_t;
  typedef nall::uint_t<26> uint26_t;
  typedef nall::uint_t<27> uint27_t;
  typedef nall::uint_t<28> uint28_t;
  typedef nall::uint_t<29> uint29_t;
  typedef nall::uint_t<30> uint30_t;
  typedef nall::uint_t<31> uint31_t;
//typedef nall::uint_t<32> uint32_t;

  typedef nall::int_t< 1>  int1_t;
  typedef nall::int_t< 2>  int2_t;
  typedef nall::int_t< 3>  int3_t;
  typedef nall::int_t< 4>  int4_t;
  typedef nall::int_t< 5>  int5_t;
  typedef nall::int_t< 6>  int6_t;
  typedef nall::int_t< 7>  int7_t;
//typedef nall::int_t< 8>  int8_t;

  typedef nall::int_t< 9>  int9_t;
  typedef nall::int_t<10> int10_t;
  typedef nall::int_t<11> int11_t;
  typedef nall::int_t<12> int12_t;
  typedef nall::int_t<13> int13_t;
  typedef nall::int_t<14> int14_t;
  typedef nall::int_t<15> int15_t;
//typedef nall::int_t<16> int16_t;

  typedef nall::int_t<17> int17_t;
  typedef nall::int_t<18> int18_t;
  typedef nall::int_t<19> int19_t;
  typedef nall::int_t<20> int20_t;
  typedef nall::int_t<21> int21_t;
  typedef nall::int_t<22> int22_t;
  typedef nall::int_t<23> int23_t;
  typedef nall::int_t<24> int24_t;
  typedef nall::int_t<25> int25_t;
  typedef nall::int_t<26> int26_t;
  typedef nall::int_t<27> int27_t;
  typedef nall::int_t<28> int28_t;
  typedef nall::int_t<29> int29_t;
  typedef nall::int_t<30> int30_t;
  typedef nall::int_t<31> int31_t;
//typedef nall::int_t<32> int32_t;

#endif
