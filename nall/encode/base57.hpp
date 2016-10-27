#pragma once

#include <nall/arithmetic.hpp>

namespace nall { namespace Encode {

template<typename T> inline auto Base57(T value) -> string {
  static const char lookup[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
  static const uint size = ceil(sizeof(T) * 8 / log2(57));

  string result;
  for(auto n : range(size)) {
    result.append(lookup[value % 57]);
    value /= 57;
  }
  return result;
}

}}
