#ifndef NALL_VARINT_HPP
#define NALL_VARINT_HPP

#include <nall/bit.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>

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

using int1 = nall::int_t<1>;
using int2 = nall::int_t<2>;
using int3 = nall::int_t<3>;
using int4 = nall::int_t<4>;
using int5 = nall::int_t<5>;
using int6 = nall::int_t<6>;
using int7 = nall::int_t<7>;

using int9 = nall::int_t<9>;
using int10 = nall::int_t<10>;
using int11 = nall::int_t<11>;
using int12 = nall::int_t<12>;
using int13 = nall::int_t<13>;
using int14 = nall::int_t<14>;
using int15 = nall::int_t<15>;

using int17 = nall::int_t<17>;
using int18 = nall::int_t<18>;
using int19 = nall::int_t<19>;
using int20 = nall::int_t<20>;
using int21 = nall::int_t<21>;
using int22 = nall::int_t<22>;
using int23 = nall::int_t<23>;
using int24 = nall::int_t<24>;
using int25 = nall::int_t<25>;
using int26 = nall::int_t<26>;
using int27 = nall::int_t<27>;
using int28 = nall::int_t<28>;
using int29 = nall::int_t<29>;
using int30 = nall::int_t<30>;
using int31 = nall::int_t<31>;

using int33 = nall::int_t<33>;
using int34 = nall::int_t<34>;
using int35 = nall::int_t<35>;
using int36 = nall::int_t<36>;
using int37 = nall::int_t<37>;
using int38 = nall::int_t<38>;
using int39 = nall::int_t<39>;
using int40 = nall::int_t<40>;
using int41 = nall::int_t<41>;
using int42 = nall::int_t<42>;
using int43 = nall::int_t<43>;
using int44 = nall::int_t<44>;
using int45 = nall::int_t<45>;
using int46 = nall::int_t<46>;
using int47 = nall::int_t<47>;
using int48 = nall::int_t<48>;
using int49 = nall::int_t<49>;
using int50 = nall::int_t<50>;
using int51 = nall::int_t<51>;
using int52 = nall::int_t<52>;
using int53 = nall::int_t<53>;
using int54 = nall::int_t<54>;
using int55 = nall::int_t<55>;
using int56 = nall::int_t<56>;
using int57 = nall::int_t<57>;
using int58 = nall::int_t<58>;
using int59 = nall::int_t<59>;
using int60 = nall::int_t<60>;
using int61 = nall::int_t<61>;
using int62 = nall::int_t<62>;
using int63 = nall::int_t<63>;

using uint1 = nall::uint_t<1>;
using uint2 = nall::uint_t<2>;
using uint3 = nall::uint_t<3>;
using uint4 = nall::uint_t<4>;
using uint5 = nall::uint_t<5>;
using uint6 = nall::uint_t<6>;
using uint7 = nall::uint_t<7>;

using uint9 = nall::uint_t<9>;
using uint10 = nall::uint_t<10>;
using uint11 = nall::uint_t<11>;
using uint12 = nall::uint_t<12>;
using uint13 = nall::uint_t<13>;
using uint14 = nall::uint_t<14>;
using uint15 = nall::uint_t<15>;

using uint17 = nall::uint_t<17>;
using uint18 = nall::uint_t<18>;
using uint19 = nall::uint_t<19>;
using uint20 = nall::uint_t<20>;
using uint21 = nall::uint_t<21>;
using uint22 = nall::uint_t<22>;
using uint23 = nall::uint_t<23>;
using uint24 = nall::uint_t<24>;
using uint25 = nall::uint_t<25>;
using uint26 = nall::uint_t<26>;
using uint27 = nall::uint_t<27>;
using uint28 = nall::uint_t<28>;
using uint29 = nall::uint_t<29>;
using uint30 = nall::uint_t<30>;
using uint31 = nall::uint_t<31>;

using uint33 = nall::uint_t<33>;
using uint34 = nall::uint_t<34>;
using uint35 = nall::uint_t<35>;
using uint36 = nall::uint_t<36>;
using uint37 = nall::uint_t<37>;
using uint38 = nall::uint_t<38>;
using uint39 = nall::uint_t<39>;
using uint40 = nall::uint_t<40>;
using uint41 = nall::uint_t<41>;
using uint42 = nall::uint_t<42>;
using uint43 = nall::uint_t<43>;
using uint44 = nall::uint_t<44>;
using uint45 = nall::uint_t<45>;
using uint46 = nall::uint_t<46>;
using uint47 = nall::uint_t<47>;
using uint48 = nall::uint_t<48>;
using uint49 = nall::uint_t<49>;
using uint50 = nall::uint_t<50>;
using uint51 = nall::uint_t<51>;
using uint52 = nall::uint_t<52>;
using uint53 = nall::uint_t<53>;
using uint54 = nall::uint_t<54>;
using uint55 = nall::uint_t<55>;
using uint56 = nall::uint_t<56>;
using uint57 = nall::uint_t<57>;
using uint58 = nall::uint_t<58>;
using uint59 = nall::uint_t<59>;
using uint60 = nall::uint_t<60>;
using uint61 = nall::uint_t<61>;
using uint62 = nall::uint_t<62>;
using uint63 = nall::uint_t<63>;

#endif
