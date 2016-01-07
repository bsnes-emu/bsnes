#pragma once

#include <nall/range.hpp>

namespace nall {
struct string;
namespace Hash {

struct CRC16 {
  CRC16() { reset(); }
  CRC16(const void* values, unsigned size) : CRC16() { data(values, size); }

  auto reset() -> void {
    checksum = ~0;
  }

  auto data(uint8_t value) -> void {
    for(auto n : range(8)) {
      if((checksum & 1) ^ (value & 1)) checksum = (checksum >> 1) ^ 0x8408;
      else checksum >>= 1;
      value >>= 1;
    }
  }

  auto data(const void* values, unsigned size) -> void {
    auto p = (const uint8_t*)values;
    while(size--) data(*p++);
  }

  auto value() -> uint16_t {
    return ~checksum;
  }

  inline auto digest() -> string;

private:
  uint16_t checksum;
};

}}
