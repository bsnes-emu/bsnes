#pragma once

//move to front

namespace nall { namespace Encode {

inline auto MTF(const void* data, uint size) -> vector<uint8_t> {
  auto input = (const uint8_t*)data;
  vector<uint8_t> output;
  output.resize(size);

  uint8_t order[256];
  for(uint n : range(256)) order[n] = n;

  for(uint offset = 0; offset < size; offset++) {
    auto data = input[offset];
    for(uint index = 0; index < 256; index++) {
      uint value = order[index];
      if(value == data) {
        output[offset] = index;
        memory::move(&order[1], &order[0], index);
        order[0] = value;
        break;
      }
    }
  }

  return output;
}

template<typename T>
inline auto MTF(const vector<T>& buffer) -> vector<uint8_t> {
  return move(MTF(buffer.data(), buffer.size() * sizeof(T)));
}

}}
