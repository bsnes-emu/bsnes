#pragma once

#include <nall/arithmetic.hpp>

namespace nall { namespace Decode {

template<typename T> inline auto Base57(const string& value) -> T {
  T result = 0;
  for(auto n : rrange(value.size())) {
    auto byte = value[n];
         if(byte >= '2' && byte <= '9') byte -= '2' -  0;
    else if(byte >= 'A' && byte <= 'H') byte -= 'A' -  8;
    else if(byte >= 'J' && byte <= 'N') byte -= 'J' - 16;
    else if(byte >= 'P' && byte <= 'Z') byte -= 'P' - 21;
    else if(byte >= 'a' && byte <= 'k') byte -= 'a' - 32;
    else if(byte >= 'm' && byte <= 'z') byte -= 'm' - 43;
    else return 0;
    result = result * 57 + byte;
  }
  return result;
}

}}
