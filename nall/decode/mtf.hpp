#pragma once

//move to front

namespace nall { namespace Decode {

inline auto MTF(const void* data, uint size) -> vector<uint8_t> {
  auto input = (const uint8_t*)data;
  vector<uint8_t> output;
  output.resize(size);

  uint8_t order[256];
  for(uint n : range(256)) order[n] = n;

  for(uint offset = 0; offset < size; offset++) {
    auto data = input[offset];
    for(uint index = 0; index < 256; index++) {
      uint value = order[data];
      if(value == index) {
        output[offset] = value;
        memory::move(&order[1], &order[0], index);
        order[0] = index;
        break;
      }
    }
  }

  return output;
}

}}
