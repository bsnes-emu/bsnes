#include <gba/gba.hpp>

namespace GBA {

Bus bus;

struct UnmappedMemory : Memory {
  uint32 read(uint32 addr, uint32 size) { print(hex<8>(addr), ":", decimal<2>(size), "\n"); return 0u; }
  void write(uint32 addr, uint32 size, uint32 word) { print(hex<8>(addr), ":", decimal<2>(size), "=", hex<8>(word), "\n"); }
};

static UnmappedMemory unmappedMemory;

uint32 StaticMemory::read(uint32 addr, uint32 size) {
  uint32 word = 0;

  switch(size) {
  case Word:
    addr &= ~3;
    word |= data[addr + 0] <<  0;
    word |= data[addr + 1] <<  8;
    word |= data[addr + 2] << 16;
    word |= data[addr + 3] << 24;
    break;
  case Half:
    addr &= ~1;
    word |= data[addr + 0] <<  0;
    word |= data[addr + 1] <<  8;
    break;
  case Byte:
    word |= data[addr + 0] <<  0;
    break;
  }

  return word;
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

uint32 Bus::read(uint32 addr, uint32 size) {
  switch(addr & 0x0f000000) {
  case 0x00000000: return system.bios.read(addr & 0x3fff, size);
  case 0x01000000: return system.bios.read(addr & 0x3fff, size);
  case 0x02000000: return cpu.ewram.read(addr & 0x3ffff, size);
  case 0x03000000: return cpu.iwram.read(addr & 0x7fff, size);
  case 0x04000000: return mmio[addr & 0x3ff]->read(addr, size);
  case 0x05000000: return ppu.pram.read(addr & 0x3ff, size);
  case 0x06000000: return ppu.vram.read(addr & 0x10000 ? (0x10000 + (addr & 0x7fff)) : (addr & 0xffff), size);
  case 0x07000000: return ppu.oam.read(addr & 0x3ff, size);
  case 0x08000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x09000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x0a000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x0b000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x0c000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x0d000000: return cartridge.rom.read(addr & 0x1ffffff, size);
  case 0x0e000000: return cartridge.ram.read(addr & 0xffff, size);
  case 0x0f000000: return cartridge.ram.read(addr & 0xffff, size);
  }
}

void Bus::write(uint32 addr, uint32 size, uint32 word) {
  switch(addr & 0x0f000000) {
  case 0x00000000: return;
  case 0x01000000: return;
  case 0x02000000: return cpu.ewram.write(addr & 0x3ffff, size, word);
  case 0x03000000: return cpu.iwram.write(addr & 0x7fff, size, word);
  case 0x04000000: return mmio[addr & 0x3ff]->write(addr, size, word);
  case 0x05000000: return ppu.pram.write(addr & 0x3ff, size, word);
  case 0x06000000: return ppu.vram.write(addr & 0x10000 ? (0x10000 + (addr & 0x7fff)) : (addr & 0xffff), size, word);
  case 0x07000000: return ppu.oam.write(addr & 0x3ff, size, word);
  case 0x08000000: return;
  case 0x09000000: return;
  case 0x0a000000: return;
  case 0x0b000000: return;
  case 0x0c000000: return;
  case 0x0d000000: return;
  case 0x0e000000: return cartridge.ram.write(addr & 0xffff, size, word);
  case 0x0f000000: return cartridge.ram.write(addr & 0xffff, size, word);
  }
}

void Bus::power() {
  for(unsigned n = 0; n < 0x400; n++) mmio[n] = &unmappedMemory;
}

}
