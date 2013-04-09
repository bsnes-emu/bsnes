#ifndef NALL_CRC16_HPP
#define NALL_CRC16_HPP

#include <nall/stdint.hpp>

namespace nall {
  inline uint16_t crc16_adjust(uint16_t crc16, uint8_t data) {
    for(unsigned n = 0; n < 8; n++) {
      if((crc16 & 1) ^ (data & 1)) crc16 = (crc16 >> 1) ^ 0x8408;
      else crc16 >>= 1;
      data >>= 1;
    }
    return crc16;
  }

  inline uint16_t crc16_calculate(const uint8_t *data, unsigned length) {
    uint16_t crc16 = ~0;
    for(unsigned n = 0; n < length; n++) {
      crc16 = crc16_adjust(crc16, data[n]);
    }
    return ~crc16;
  }
}

#endif
