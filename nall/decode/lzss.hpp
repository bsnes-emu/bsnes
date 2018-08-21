#pragma once

namespace nall { namespace Decode {

inline auto LZSS(const void* data) -> vector<uint8_t> {
  vector<uint8_t> output;

  auto input = (const uint8_t*)data;
  uint64_t size = 0;
  for(uint byte : range(8)) size |= *input++ << byte * 8;
  output.resize(size);
  const uint windowBits = *input++;
  const uint lengthBits = *input++;

  const uint lengthExtends = 4 + (1 << lengthBits) - 1;
  const uint windowMask = (1 << windowBits) - 1;

  for(uint offset = 0, flags = 0, bit = 7; offset < size;) {
    if(++bit == 8) bit = 0, flags = *input++;

    if(flags & 1 << bit) {
      uint encoding = 0;
      encoding |= *input++ <<  0;
      encoding |= *input++ <<  8;
      encoding |= *input++ << 16;

      uint length = 4 + (encoding >> windowBits);
      uint window = 1 + (encoding & windowMask);
      if(length == lengthExtends) length += *input++;

      for(uint index : range(length)) {
        if(offset + index >= size) break;
        output[offset + index] = output[offset + index - window];
      }
      offset += length;
    } else {
      output[offset++] = *input++;
    }
  }

  return output;
}

}}
