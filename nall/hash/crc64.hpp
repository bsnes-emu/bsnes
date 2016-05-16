#pragma once

#include <nall/range.hpp>
#include <nall/string.hpp>

namespace nall { namespace Hash {

struct CRC64 {
  CRC64() { reset(); }
  CRC64(const void* values, uint size) : CRC64() { data(values, size); }
  CRC64(const vector<uint8_t>& values) : CRC64() { data(values); }
  CRC64(const string& values) : CRC64() { data(values); }

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

  auto data(const string& values) -> void {
    for(auto value : values) data(value);
  }

  auto value() const -> uint64_t {
    return ~checksum;
  }

  inline auto digest() const -> string {
    return hex(value(), 16L);
  }

private:
  static auto table(uint8_t index) -> uint64_t {
    static uint64_t table[256] = {0};
    static bool initialized = false;

    if(!initialized) {
      initialized = true;
      for(auto index : range(256)) {
        uint64_t crc = index;
        for(auto bit : range(8)) {
          crc = (crc >> 1) ^ (crc & 1 ? 0xc96c'5795'd787'0f42 : 0);
        }
        table[index] = crc;
      }
    }

    return table[index];
  }

  uint64_t checksum;
};

}}
