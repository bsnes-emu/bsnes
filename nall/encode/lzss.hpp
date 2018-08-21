#pragma once

#include <nall/encode/dictionary.hpp>

namespace nall { namespace Encode {

inline auto LZSS(const void* data, uint64_t size, uint windowBits = 16, uint lengthBits = 8) -> vector<uint8_t> {
  vector<uint8_t> output;
  for(uint byte : range(8)) output.append(size >> byte * 8);
  output.append(windowBits);
  output.append(lengthBits);

  const uint lengthExtends = 4 + (1 << lengthBits) - 1;
  const uint lengthMaximum = lengthExtends + 255;
  const uint windowMaximum = 1 << windowBits;
  const uint windowRange = windowMaximum + lengthMaximum;

  auto input = (const uint8_t*)data;
  auto read = [&](uint address) -> uint {
    if(address + 3 > size) return 0;
    return input[address + 0] << 24 | input[address + 1] << 16 | input[address + 2] << 8 | input[address + 3] << 0;
  };

  Dictionary dictionary(data, size, 2 * windowRange);
  dictionary.scan();

  for(uint offset = 0, base = 0, flags = 0, bit = 7; offset < size;) {
    if(offset - base >= 2 * windowRange) {
      dictionary.scan(base = offset - windowRange);
    }

    uint prefix = read(offset), lower, upper;
    dictionary.find(prefix, lower, upper);

    uint lengthLongest = 0, windowLongest = 0;
    for(uint index = lower; index <= upper; index++) {
      int window = (int)offset - (int)dictionary[index];
      if(window <= 0) continue;
      window = min(window, windowMaximum);

      uint length = 0;
      do {
        if(offset + length >= size) break;
        if(input[offset + length] != input[offset + length - window]) break;
      } while(++length < lengthMaximum);

      if(length > lengthLongest) {
        lengthLongest = length;
        windowLongest = window;
        if(length == lengthMaximum) break;
      }
    }

    if(++bit == 8) {
      flags = output.size();
      output.append(bit = 0);
    }

    if(lengthLongest < 4) {
      output.append(input[offset++]);
    } else {
      output[flags] |= 1 << bit;
      offset += lengthLongest;

      uint encoding = min(lengthLongest, lengthExtends) - 4 << windowBits | windowLongest - 1;
      output.append(encoding >>  0);
      output.append(encoding >>  8);
      output.append(encoding >> 16);
      if(lengthLongest >= lengthExtends) output.append(lengthLongest - lengthExtends);
    }
  }

  return output;
}

}}
