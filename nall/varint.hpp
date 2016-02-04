#pragma once

#include <nall/primitives.hpp>
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

using  int1 = nall::Integer< 1>;
using  int2 = nall::Integer< 2>;
using  int3 = nall::Integer< 3>;
using  int4 = nall::Integer< 4>;
using  int5 = nall::Integer< 5>;
using  int6 = nall::Integer< 6>;
using  int7 = nall::Integer< 7>;
using  int8_= nall::Integer< 8>;
using  int9 = nall::Integer< 9>;
using int10 = nall::Integer<10>;
using int11 = nall::Integer<11>;
using int12 = nall::Integer<12>;
using int13 = nall::Integer<13>;
using int14 = nall::Integer<14>;
using int15 = nall::Integer<15>;
using int16_= nall::Integer<16>;
using int17 = nall::Integer<17>;
using int18 = nall::Integer<18>;
using int19 = nall::Integer<19>;
using int20 = nall::Integer<20>;
using int21 = nall::Integer<21>;
using int22 = nall::Integer<22>;
using int23 = nall::Integer<23>;
using int24 = nall::Integer<24>;
using int25 = nall::Integer<25>;
using int26 = nall::Integer<26>;
using int27 = nall::Integer<27>;
using int28 = nall::Integer<28>;
using int29 = nall::Integer<29>;
using int30 = nall::Integer<30>;
using int31 = nall::Integer<31>;
using int32_= nall::Integer<32>;
using int33 = nall::Integer<33>;
using int34 = nall::Integer<34>;
using int35 = nall::Integer<35>;
using int36 = nall::Integer<36>;
using int37 = nall::Integer<37>;
using int38 = nall::Integer<38>;
using int39 = nall::Integer<39>;
using int40 = nall::Integer<40>;
using int41 = nall::Integer<41>;
using int42 = nall::Integer<42>;
using int43 = nall::Integer<43>;
using int44 = nall::Integer<44>;
using int45 = nall::Integer<45>;
using int46 = nall::Integer<46>;
using int47 = nall::Integer<47>;
using int48 = nall::Integer<48>;
using int49 = nall::Integer<49>;
using int50 = nall::Integer<50>;
using int51 = nall::Integer<51>;
using int52 = nall::Integer<52>;
using int53 = nall::Integer<53>;
using int54 = nall::Integer<54>;
using int55 = nall::Integer<55>;
using int56 = nall::Integer<56>;
using int57 = nall::Integer<57>;
using int58 = nall::Integer<58>;
using int59 = nall::Integer<59>;
using int60 = nall::Integer<60>;
using int61 = nall::Integer<61>;
using int62 = nall::Integer<62>;
using int63 = nall::Integer<63>;
using int64_= nall::Integer<64>;

using  uint1 = nall::Natural< 1>;
using  uint2 = nall::Natural< 2>;
using  uint3 = nall::Natural< 3>;
using  uint4 = nall::Natural< 4>;
using  uint5 = nall::Natural< 5>;
using  uint6 = nall::Natural< 6>;
using  uint7 = nall::Natural< 7>;
using  uint8_= nall::Natural< 8>;
using  uint9 = nall::Natural< 9>;
using uint10 = nall::Natural<10>;
using uint11 = nall::Natural<11>;
using uint12 = nall::Natural<12>;
using uint13 = nall::Natural<13>;
using uint14 = nall::Natural<14>;
using uint15 = nall::Natural<15>;
using uint16_= nall::Natural<16>;
using uint17 = nall::Natural<17>;
using uint18 = nall::Natural<18>;
using uint19 = nall::Natural<19>;
using uint20 = nall::Natural<20>;
using uint21 = nall::Natural<21>;
using uint22 = nall::Natural<22>;
using uint23 = nall::Natural<23>;
using uint24 = nall::Natural<24>;
using uint25 = nall::Natural<25>;
using uint26 = nall::Natural<26>;
using uint27 = nall::Natural<27>;
using uint28 = nall::Natural<28>;
using uint29 = nall::Natural<29>;
using uint30 = nall::Natural<30>;
using uint31 = nall::Natural<31>;
using uint32_= nall::Natural<32>;
using uint33 = nall::Natural<33>;
using uint34 = nall::Natural<34>;
using uint35 = nall::Natural<35>;
using uint36 = nall::Natural<36>;
using uint37 = nall::Natural<37>;
using uint38 = nall::Natural<38>;
using uint39 = nall::Natural<39>;
using uint40 = nall::Natural<40>;
using uint41 = nall::Natural<41>;
using uint42 = nall::Natural<42>;
using uint43 = nall::Natural<43>;
using uint44 = nall::Natural<44>;
using uint45 = nall::Natural<45>;
using uint46 = nall::Natural<46>;
using uint47 = nall::Natural<47>;
using uint48 = nall::Natural<48>;
using uint49 = nall::Natural<49>;
using uint50 = nall::Natural<50>;
using uint51 = nall::Natural<51>;
using uint52 = nall::Natural<52>;
using uint53 = nall::Natural<53>;
using uint54 = nall::Natural<54>;
using uint55 = nall::Natural<55>;
using uint56 = nall::Natural<56>;
using uint57 = nall::Natural<57>;
using uint58 = nall::Natural<58>;
using uint59 = nall::Natural<59>;
using uint60 = nall::Natural<60>;
using uint61 = nall::Natural<61>;
using uint62 = nall::Natural<62>;
using uint63 = nall::Natural<63>;
using uint64_= nall::Natural<64>;
