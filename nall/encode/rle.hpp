#pragma once

namespace nall { namespace Encode {

template<typename T> inline auto RLE(const void* data_, uint size, uint minimum = 0) -> vector<uint8_t> {
  if(!minimum) minimum = max(1, 4 / sizeof(T));
  vector<uint8_t> result;

  auto data = (const T*)data_;
  uint base = 0;
  uint skip = 0;

  for(uint byte : range(sizeof(uint))) result.append(size * sizeof(T) >> byte * 8);

  auto read = [&](uint offset) -> T {
    if(offset >= size) return {};
    return data[offset];
  };

  auto write = [&](T value) -> void {
    for(uint byte : range(sizeof(T))) result.append(value >> byte * 8);
  };

  auto flush = [&]() -> void {
    result.append(skip - 1);
    do { write(read(base++)); } while(--skip);
  };

  while(base + skip < size) {
    uint same = 1;
    for(uint offset = base + skip + 1; offset < size; offset++) {
      if(read(offset) != read(base + skip)) break;
      if(++same == 127 + minimum) break;
    }

    if(same < minimum) {
      if(++skip == 128) flush();
    } else {
      if(skip) flush();
      result.append(128 | same - minimum);
      write(read(base));
      base += same;
    }
  }
  if(skip) flush();

  return result;
}

}}
