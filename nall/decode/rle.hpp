#pragma once

namespace nall { namespace Decode {

template<typename T> inline auto RLE(const uint8_t* data, uint remaining = ~0, uint minimum = 0) -> vector<T> {
  if(!minimum) minimum = max(1, 4 / sizeof(T));
  vector<T> result;

  auto load = [&]() -> uint8_t {
    if(!remaining) return 0x00;
    return --remaining, *data++;
  };

  uint base = 0;
  uint size = 0;
  for(uint byte : range(sizeof(uint))) size |= load() << byte * 8;
  size /= sizeof(T);
  result.resize(size);

  auto read = [&]() -> T {
    T value = 0;
    for(uint byte : range(sizeof(T))) value |= load() << byte * 8;
    return value;
  };

  auto write = [&](T value) -> void {
    if(base >= size) return;
    result[base++] = value;
  };

  while(base < size) {
    auto byte = *data++;
    if(byte < 128) {
      byte++;
      while(byte--) write(read());
    } else {
      auto value = read();
      byte = (byte & 127) + minimum;
      while(byte--) write(value);
    }
  }

  return result;
}

}}
