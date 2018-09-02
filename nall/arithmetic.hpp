#pragma once

//multi-precision arithmetic
//warning: each size is quadratically more expensive than the size before it!

#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/range.hpp>
#include <nall/traits.hpp>

#include <nall/arithmetic/unsigned.hpp>

#if !defined(__SIZEOF_INT128__)
#define PairBits 128
#define TypeBits  64
#define HalfBits  32
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits
#endif

#define PairBits 256
#define TypeBits 128
#define HalfBits  64
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

#define PairBits 512
#define TypeBits 256
#define HalfBits 128
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

#define PairBits 1024
#define TypeBits  512
#define HalfBits  256
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

#define PairBits 2048
#define TypeBits 1024
#define HalfBits  512
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

#define PairBits 4096
#define TypeBits 2048
#define HalfBits 1024
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

#define PairBits 8192
#define TypeBits 4096
#define HalfBits 2048
#include <nall/arithmetic/natural.hpp>
#undef PairBits
#undef TypeBits
#undef HalfBits

namespace nall {
  //TODO: these types are for expressing smaller bit ranges in class interfaces
  //for instance, XChaCha20 taking a 192-bit nonce
  //however, they still allow more bits than expressed ...
  //some sort of wrapper needs to be devised to ensure these sizes are masked and wrap appropriately

  using uint192_t = uint256_t;
}
