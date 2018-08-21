#pragma once

//burrows-wheeler transform

#include <nall/suffix-array.hpp>

namespace nall { namespace Encode {

inline auto BWT(const void* data, uint size) -> vector<uint8_t> {
  auto input = (const uint8_t*)data;
  vector<uint8_t> output;
  output.reserve(8 + 8 + size);
  for(uint byte : range(8)) output.append(size >> byte * 8);
  for(uint byte : range(8)) output.append(0x00);

  auto suffixes = new int[size];
//suffix_array(suffixes, input, size);
  for(uint n : range(size)) suffixes[n] = n;
  sort(suffixes, size, [&](int lhs, int rhs) -> bool {
    uint l = size;
    while(l--) {
      auto x = input[lhs++];
      auto y = input[rhs++];
      if(x != y) return x - y < 0;
      if(lhs >= size) lhs = 0;
      if(rhs >= size) rhs = 0;
    }
    return 0;
  });

  uint64_t root = 0;
  for(uint offset : range(size)) {
    if(suffixes[offset] == 0) root = offset;
    uint suffix = suffixes[offset];
    if(suffix == 0) suffix = size;
    output.append(input[--suffix]);
  }

  for(uint byte : range(8)) output[8 + byte] = root >> byte * 8;
  delete[] suffixes;
  return output;
}

template<typename T>
inline auto BWT(const vector<T>& buffer) -> vector<uint8_t> {
  return move(BWT(buffer.data(), buffer.size() * sizeof(T)));
}

}}
