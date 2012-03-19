#include <gba/gba.hpp>

namespace GBA {

Bus bus;

uint32 StaticMemory::read(uint32 addr, uint32 size) {
  unsigned bits = addr & 3;
  addr &= ~3;

  uint32 word = 0;
  switch(size) {
  case Word: word |= data[addr + 3] << 24;
             word |= data[addr + 2] << 16;
  case Half: word |= data[addr + 1] <<  8;
  case Byte: word |= data[addr + 0] <<  0;
  }

  if(bits) {
    unsigned rotate = bits << 3;
    word = (word >> rotate) | (word << (32 - rotate));
  }

  switch(size) {
  case Word: return word;
  case Half: return word & 0xffff;
  case Byte: return word & 0xff;
  }
}

void StaticMemory::write(uint32 addr, uint32 size, uint32 word) {
  switch(size) {
  case Word:
    addr &= ~3;
    data[addr + 3] = word >> 24;
    data[addr + 2] = word >> 16;
    data[addr + 1] = word >>  8;
    data[addr + 0] = word >>  0;
    break;
  case Half:
    addr &= ~1;
    data[addr + 1] = word >>  8;
    data[addr + 0] = word >>  0;
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
  case 0x02000000: return cpu.eram.read(addr & 0x3ffff, size);
  case 0x03000000: return cpu.iram.read(addr & 0x7fff, size);
  case 0x04000000: return 0u;  //MMIO [0x400]
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
  case 0x02000000: return cpu.eram.write(addr & 0x3ffff, size, word);
  case 0x03000000: return cpu.iram.write(addr & 0x7fff, size, word);
  case 0x04000000: return;  //MMIO [0x400]
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

}
