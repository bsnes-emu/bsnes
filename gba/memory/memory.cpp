#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "mmio.cpp"
#include "serialization.cpp"
Bus bus;

struct UnmappedMemory : Memory {
  auto read(uint32 addr, uint32 size) -> uint32 { return 0; }
  auto write(uint32 addr, uint32 size, uint32 word) -> void {}
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

auto Bus::wait(uint32 addr, uint32 size) -> unsigned {
  switch(addr & 0x0f000000) {
  case 0x00000000: return 1;
  case 0x01000000: return 1;
  case 0x02000000: return (16 - cpu.regs.memory.control.ewramwait) * (size == Word ? 2 : 1);
  case 0x03000000: return 1;
  case 0x04000000: return 2;
  case 0x05000000: return 1 + (size == Word);
  case 0x06000000: return 1 + (size == Word);
  case 0x07000000: return 1;
  default: {
    unsigned n = cpu.regs.wait.control.nwait[addr >> 25 & 3];
    unsigned s = cpu.regs.wait.control.swait[addr >> 25 & 3];

    static unsigned timing[] = {5, 4, 3, 9};
    n = timing[n];

    switch(addr & 0x0e000000) {
    case 0x08000000: s = s ? 2 : 3; break;
    case 0x0a000000: s = s ? 2 : 5; break;
    case 0x0c000000: s = s ? 2 : 9; break;
    case 0x0e000000: s = n; break;
    }

    bool sequential = cpu.sequential();
    if((addr & 0x1fffe) == 0) sequential = false;  //N cycle on 16-bit ROM crossing 128KB page boundary (RAM S==N)
    if(idleflag) sequential = false;  //LDR/LDM interrupts instruction fetches

    unsigned clocks = sequential ? s : n;
    if(size == Word) clocks += s;  //16-bit bus requires two transfers for words
    return clocks;
  }
  }
}

auto Bus::idle(uint32 addr) -> void {
  if(addr & 0x08000000) idleflag = true;
}

auto Bus::read(uint32 addr, uint32 size) -> uint32 {
  idleflag = false;
  if(addr & 0x08000000) return cartridge.read(addr, size);

  switch(addr >> 24 & 7) {
  case 0: return bios.read(addr, size);
  case 1: return bios.read(addr, size);
  case 2: return cpu.ewram_read(addr, size);
  case 3: return cpu.iwram_read(addr, size);
  case 4:
    if((addr & 0xfffffc00) == 0x04000000) return mmio[addr & 0x3ff]->read(addr, size);
    if((addr & 0xff00ffff) == 0x04000800) return ((MMIO&)cpu).read(0x04000800 | (addr & 3), size);
    return cpu.pipeline.fetch.instruction;
  case 5: return ppu.pram_read(addr, size);
  case 6: return ppu.vram_read(addr, size);
  case 7: return ppu.oam_read(addr, size);
  }
}

auto Bus::write(uint32 addr, uint32 size, uint32 word) -> void {
  idleflag = false;
  if(addr & 0x08000000) return cartridge.write(addr, size, word);

  switch(addr >> 24 & 7) {
  case 0: return;
  case 1: return;
  case 2: return cpu.ewram_write(addr, size, word);
  case 3: return cpu.iwram_write(addr, size, word);
  case 4:
    if((addr & 0xfffffc00) == 0x04000000) return mmio[addr & 0x3ff]->write(addr, size, word);
    if((addr & 0xff00ffff) == 0x04000800) return ((MMIO&)cpu).write(0x04000800 | (addr & 3), size, word);
    return;
  case 5: return ppu.pram_write(addr, size, word);
  case 6: return ppu.vram_write(addr, size, word);
  case 7: return ppu.oam_write(addr, size, word);
  }
}

auto Bus::power() -> void {
  for(unsigned n = 0; n < 0x400; n++) mmio[n] = &unmappedMemory;
  idleflag = false;
}

}
