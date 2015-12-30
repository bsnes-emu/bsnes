#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "mmio.cpp"
Bus bus;

struct UnmappedMemory : Memory {
  auto read(uint mode, uint32 addr) -> uint32 override { return 0; }
  auto write(uint mode, uint32 addr, uint32 word) -> void override {}
};

static UnmappedMemory unmappedMemory;

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
  for(auto n : range(0x400)) mmio[n] = &unmappedMemory;
}

}
