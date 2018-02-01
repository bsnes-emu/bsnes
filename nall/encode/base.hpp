#pragma once

#include <nall/arithmetic.hpp>

namespace nall { namespace Encode {

template<uint Bits, typename T> inline auto Base(T value) -> string {
  static const string format =
    Bits ==  2 ? "01"
  : Bits ==  8 ? "01234567"
  : Bits == 10 ? "0123456789"
  : Bits == 16 ? "0123456789abcdef"
  : Bits == 32 ? "0123456789abcdefghijklmnopqrstuv"
  : Bits == 34 ? "023456789abcdefghijkmnopqrstuvwxyz"  //1l
  : Bits == 36 ? "0123456789abcdefghijklmnopqrstuvwxyz"
  : Bits == 57 ? "23456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"  //01IOl
  : Bits == 62 ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
  : Bits == 64 ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}"
  : Bits == 85 ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%()+,-.:;=@[]^_`{|}~"  //\ "&'*/<>?
  : "";
  static const uint size = ceil(sizeof(T) * 8 / log2(Bits));

  string result;
  result.resize(size);
  char* data = result.get() + size;
  for(auto byte : result) {
    *--data = format[value % Bits];
    value /= Bits;
  }
  return result;
}

}}
