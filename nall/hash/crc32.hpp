#pragma once

#include <nall/range.hpp>

namespace nall { struct string; }

namespace nall { namespace Hash {

struct CRC32 {
  CRC32() { reset(); }
  CRC32(const void* values, uint size) : CRC32() { data(values, size); }
  CRC32(const vector<uint8_t>& values) : CRC32() { data(values); }

  auto reset() -> void {
    checksum = ~0;
  }

  auto data(uint8_t value) -> void {
    checksum = (checksum >> 8) ^ table(checksum ^ value);
  }

  auto data(const void* values, uint size) -> void {
    auto p = (const uint8_t*)values;
    while(size--) data(*p++);
  }

  auto data(const vector<uint8_t>& values) -> void {
    for(auto value : values) data(value);
  }

  auto value() const -> uint32_t {
    return ~checksum;
  }

  inline auto digest() const -> string;

private:
  static auto table(uint8_t index) -> uint32_t {
    static uint32_t table[256] = {0};
    static bool initialized = false;

    if(!initialized) {
      initialized = true;
      for(auto index : range(256)) {
        uint32_t crc = index;
        for(auto bit : range(8)) {
          crc = (crc >> 1) ^ (crc & 1 ? 0xedb8'8320 : 0);
        }
        table[index] = crc;
      }
    }

    return table[index];
  }

  uint32_t checksum;
};

}}
