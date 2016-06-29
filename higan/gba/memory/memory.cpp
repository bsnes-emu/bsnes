#include <gba/gba.hpp>

namespace GameBoyAdvance {

Bus bus;

auto IO::readIO(uint mode, uint32 addr) -> uint32 {
  uint32 word = 0;

  if(mode & Word) {
    addr &= ~3;
    word |= readIO(addr + 0) <<  0;
    word |= readIO(addr + 1) <<  8;
    word |= readIO(addr + 2) << 16;
    word |= readIO(addr + 3) << 24;
  } else if(mode & Half) {
    addr &= ~1;
    word |= readIO(addr + 0) <<  0;
    word |= readIO(addr + 1) <<  8;
  } else if(mode & Byte) {
    word |= readIO(addr + 0) <<  0;
  }

  return word;
}

auto IO::writeIO(uint mode, uint32 addr, uint32 word) -> void {
  if(mode & Word) {
    addr &= ~3;
    writeIO(addr + 0, word >>  0);
    writeIO(addr + 1, word >>  8);
    writeIO(addr + 2, word >> 16);
    writeIO(addr + 3, word >> 24);
  } else if(mode & Half) {
    addr &= ~1;
    writeIO(addr + 0, word >>  0);
    writeIO(addr + 1, word >>  8);
  } else if(mode & Byte) {
    writeIO(addr + 0, word >>  0);
  }
}

struct UnmappedIO : IO {
  auto readIO(uint32 addr) -> uint8 override { return 0; }
  auto writeIO(uint32 addr, uint8 byte) -> void override {}
};

static UnmappedIO unmappedIO;

auto Bus::mirror(uint32 addr, uint32 size) -> uint32 {
  uint32 base = 0;
  if(size) {
    uint32 mask = 1 << 27;  //28-bit bus
    while(addr >= size) {
      while(!(addr & mask)) mask >>= 1;
      addr -= mask;
      if(size > mask) {
        size -= mask;
        base += mask;
      }
      mask >>= 1;
    }
    base += addr;
  }
  return base;
}

auto Bus::power() -> void {
  for(auto n : range(0x400)) io[n] = &unmappedIO;
}

}
