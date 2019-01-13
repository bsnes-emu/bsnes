#pragma once

#include <nall/serializer.hpp>
#include <nall/traits.hpp>

namespace nall {
  struct Boolean;
  template<int Precision> struct Natural;
  template<int Precision> struct Integer;
}

#include <nall/primitives/bit-range.hpp>
#include <nall/primitives/boolean.hpp>
#include <nall/primitives/natural.hpp>
#include <nall/primitives/integer.hpp>
#include <nall/primitives/real.hpp>
#include <nall/primitives/types.hpp>
//#include <nall/primitives/operators.hpp>

namespace nall {
  template<int Bits> auto Natural<Bits>::integer() const -> Integer<Bits> { return Integer<Bits>(*this); }
  template<int Bits> auto Integer<Bits>::natural() const -> Natural<Bits> { return Natural<Bits>(*this); }
}
