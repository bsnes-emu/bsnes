#pragma once

#include <nall/serializer.hpp>
#include <nall/traits.hpp>

namespace nall {
  struct Boolean;
  template<uint Precision> struct Natural;
  template<uint Precision> struct Integer;
}

#include <nall/primitives/bit-range.hpp>
#include <nall/primitives/boolean.hpp>
#include <nall/primitives/natural.hpp>
#include <nall/primitives/integer.hpp>
#include <nall/primitives/real.hpp>
#include <nall/primitives/types.hpp>
#include <nall/primitives/literals.hpp>

namespace nall {
  template<uint Bits> auto Natural<Bits>::integer() const -> Integer<Bits> { return Integer<Bits>(*this); }
  template<uint Bits> auto Integer<Bits>::natural() const -> Natural<Bits> { return Natural<Bits>(*this); }
}
