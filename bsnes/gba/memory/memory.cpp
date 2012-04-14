#include <gba/gba.hpp>

namespace GBA {

#include "mmio.cpp"
Bus bus;

struct UnmappedMemory : Memory {
  uint32 read(uint32 addr, uint32 size) { return 0u; }
  void write(uint32 addr, uint32 size, uint32 word) {}
};

static UnmappedMemory unmappedMemory;

uint8& StaticMemory::operator[](uint32 addr) {
  return data[addr];
}

uint32 StaticMemory::read(uint32 addr, uint32 size) {
  switch(size) {
  case Word: addr &= ~3; return (data[addr + 0] << 0) | (data[addr + 1] << 8) | (data[addr + 2] << 16) | (data[addr + 3] << 24);
  case Half: addr &= ~1; return (data[addr + 0] << 0) | (data[addr + 1] << 8);
  case Byte:             return (data[addr + 0] << 0);
  }
}

void StaticMemory::write(uint32 addr, uint32 size, uint32 word) {
  switch(size) {
  case Word:
    addr &= ~3;
    data[addr + 0] = word >>  0;
    data[addr + 1] = word >>  8;
    data[addr + 2] = word >> 16;
    data[addr + 3] = word >> 24;
    break;
  case Half:
    addr &= ~1;
    data[addr + 0] = word >>  0;
    data[addr + 1] = word >>  8;
    break;
  case Byte:
    data[addr + 0] = word >>  0;
    break;
  }
}

StaticMemory::StaticMemory() {
  data = nullptr;
  size = 0u;
}

StaticMemory::~StaticMemory() {
  if(data) delete[] data;
}

//

uint32 Bus::mirror(uint32 addr, uint32 size) {
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

uint32 Bus::speed(uint32 addr, uint32 size) {
  return 2;

                           //B  B  E  I  M  P  V  O  R  R  R  R  R  R  S  S
                           //I  I  R  R  M  R  R  A  O  O  O  O  O  O  R  R
                           //O  O  A  A  I  A  A  M  M  M  M  M  M  M  A  A
                           //S  S  M  M  O  M  M                       M  M
  static unsigned byte[] = { 1, 1, 3, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5 };
  static unsigned half[] = { 1, 1, 3, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5 };
  static unsigned word[] = { 1, 1, 6, 1, 1, 2, 2, 1, 8, 8, 8, 8, 8, 8, 8, 8 };

  addr = (addr >> 24) & 15;
  switch(size) {
  case Byte: return byte[addr];
  case Half: return half[addr];
  case Word: return word[addr];
  }
}

uint32 Bus::read(uint32 addr, uint32 size) {
  if(addr & 0x08000000) return cartridge.read(addr, size);

  switch(addr & 0x07000000) {
  case 0x00000000: return bios.read(addr, size);
  case 0x01000000: return bios.read(addr, size);
  case 0x02000000: return cpu.ewram.read(addr & 0x3ffff, size);
  case 0x03000000: return cpu.iwram.read(addr & 0x7fff, size);
  case 0x04000000:
    if((addr & 0xfffffc00) == 0x04000000) return mmio[addr & 0x3ff]->read(addr, size);
    if((addr & 0xff00ffff) == 0x04000800) return ((MMIO&)cpu).read(0x04000800 | (addr & 3), size);
    return 0u;
  case 0x05000000: return ppu.pram_read(addr, size);
  case 0x06000000: return ppu.vram_read(addr, size);
  case 0x07000000: return ppu.oam_read(addr, size);
  }
}

void Bus::write(uint32 addr, uint32 size, uint32 word) {
  if(addr & 0x08000000) return cartridge.write(addr, size, word);

  switch(addr & 0x07000000) {
  case 0x00000000: return;
  case 0x01000000: return;
  case 0x02000000: return cpu.ewram.write(addr & 0x3ffff, size, word);
  case 0x03000000: return cpu.iwram.write(addr & 0x7fff, size, word);
  case 0x04000000:
    if((addr & 0xfffffc00) == 0x04000000) return mmio[addr & 0x3ff]->write(addr, size, word);
    if((addr & 0xff00ffff) == 0x04000800) return ((MMIO&)cpu).write(0x04000800 | (addr & 3), size, word);
    return;
  case 0x05000000: return ppu.pram_write(addr, size, word);
  case 0x06000000: return ppu.vram_write(addr, size, word);
  case 0x07000000: return ppu.oam_write(addr, size, word);
  }
}

void Bus::power() {
  for(unsigned n = 0; n < 0x400; n++) mmio[n] = &unmappedMemory;
}

}
