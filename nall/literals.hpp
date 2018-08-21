#pragma once

//note: gcc 4.9 does not support user-defined literals with arguments other than const char*
//once nall increases the minimum required GCC version, the use of nall/atoi.hpp can beremoved

#include <nall/atoi.hpp>

namespace nall { namespace Literal {

struct Capacity { uint value; };
struct Size { uint value; };

}}

namespace nall {

constexpr inline auto operator"" _capacity(const char* s) -> Literal::Capacity {
  return {(uint)toNatural(s)};
}

constexpr inline auto operator"" _size(const char* s) -> Literal::Size {
  return {(uint)toNatural(s)};
}

}
