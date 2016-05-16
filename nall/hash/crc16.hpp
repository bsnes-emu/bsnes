#pragma once

#include <nall/range.hpp>
#include <nall/string.hpp>

namespace nall { namespace Hash {

struct CRC16 {
  CRC16() { reset(); }
  CRC16(const void* values, uint size) : CRC16() { data(values, size); }
  CRC16(const vector<uint8_t>& values) : CRC16() { data(values); }
  CRC16(const string& values) : CRC16() { data(values); }

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

  auto value() const -> uint16_t {
    return ~checksum;
  }

  inline auto digest() const -> string {
    return hex(value(), 4L);
  }

private:
  static auto table(uint8_t index) -> uint16_t {
    static uint16_t table[256] = {0};
    static bool initialized = false;

    if(!initialized) {
      initialized = true;
      for(auto index : range(256)) {
        uint16_t crc = index;
        for(auto bit : range(8)) {
          crc = (crc >> 1) ^ (crc & 1 ? 0x8408 : 0);
        }
        table[index] = crc;
      }
    }

    return table[index];
  }

  uint16_t checksum;
};

}}
