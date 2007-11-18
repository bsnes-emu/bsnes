#include "../base.h"
#include "memory_rw.cpp"

namespace memory {
  MMIOAccess mmio;
  StaticRAM  wram(128 * 1024);

  UnmappedMemory memory_unmapped;
  UnmappedMMIO   mmio_unmapped;
};

uint8 UnmappedMemory::read(uint) { return cpu.regs.mdr; }
void UnmappedMemory::write(uint, uint8) {}

uint8 UnmappedMMIO::mmio_read(uint) { return cpu.regs.mdr; }
void UnmappedMMIO::mmio_write(uint, uint8) {}

void MMIOAccess::map(uint addr, MMIO &access) {
//MMIO: $[00-3f]:[2000-5fff]
  mmio[(addr - 0x2000) & 0x3fff] = &access;
}

uint8 MMIOAccess::read(uint addr) {
  return mmio[(addr - 0x2000) & 0x3fff]->mmio_read(addr);
}

void MMIOAccess::write(uint addr, uint8 data) {
  mmio[(addr - 0x2000) & 0x3fff]->mmio_write(addr, data);
}

uint Bus::mirror(uint addr, uint size) {
uint base = 0;
  if(size) {
  uint mask = 1 << 23;
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

void Bus::map(uint addr, Memory &access, uint offset) {
  page[addr >> 8].access = &access;
  page[addr >> 8].offset = offset - addr;
}

void Bus::map(
  MapMode mode,
  uint8  bank_lo, uint8  bank_hi,
  uint16 addr_lo, uint16 addr_hi,
  Memory &access, uint offset, uint size
) {
  assert(bank_lo <= bank_hi);
  assert(addr_lo <= addr_hi);

  if(access.size() == -1U) return;

uint8 page_lo = addr_lo >> 8;
uint8 page_hi = addr_hi >> 8;
uint index = 0;

  switch(mode) {
    case MapDirect: {
      for(uint bank = bank_lo; bank <= bank_hi; bank++) {
        for(uint page = page_lo; page <= page_hi; page++) {
          map((bank << 16) + (page << 8), access, (bank << 16) + (page << 8));
        }
      }
    } break;

    case MapLinear: {
      for(uint bank = bank_lo; bank <= bank_hi; bank++) {
        for(uint page = page_lo; page <= page_hi; page++) {
          map((bank << 16) + (page << 8), access, mirror(offset + index, access.size()));
          index += 256;
          if(size) index %= size;
        }
      }
    } break;

    case MapShadow: {
      for(uint bank = bank_lo; bank <= bank_hi; bank++) {
        index += page_lo * 256;
        if(size) index %= size;

        for(uint page = page_lo; page <= page_hi; page++) {
          map((bank << 16) + (page << 8), access, mirror(offset + index, access.size()));
          index += 256;
          if(size) index %= size;
        }

        index += (255 - page_hi) * 256;
        if(size) index %= size;
      }
    } break;
  }
}
