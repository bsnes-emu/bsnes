#include <snes/snes.hpp>

#define MEMORY_CPP
namespace SNES {

Bus bus;

#include "serialization.cpp"

namespace memory {
  StaticRAM wram(128 * 1024);
  StaticRAM apuram(64 * 1024);
  StaticRAM vram(64 * 1024);
  StaticRAM oam(544);
  StaticRAM cgram(512);
};

unsigned Bus::mirror(unsigned addr, unsigned size) {
  unsigned base = 0;
  if(size) {
    unsigned mask = 1 << 23;
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

void Bus::map(
  MapMode mode,
  unsigned bank_lo, unsigned bank_hi,
  unsigned addr_lo, unsigned addr_hi,
  const function<uint8 (unsigned)> &rd,
  const function<void (unsigned, uint8)> &wr,
  unsigned base, unsigned length
) {
  assert(bank_lo <= bank_hi && bank_lo <= 0xff);
  assert(addr_lo <= addr_hi && addr_lo <= 0xffff);

  unsigned page_lo = addr_lo >> 13;
  unsigned page_hi = addr_hi >> 13;
  if(length == 0) length = (bank_hi - bank_lo + 1) * (addr_hi - addr_lo + 1);

  unsigned offset = 0;
  for(unsigned bank = bank_lo; bank <= bank_hi; bank++) {
    for(unsigned page = page_lo; page <= page_hi; page++) {
      unsigned map_addr = (bank << 16) | (page << 13);
      unsigned map_page = map_addr >> 13;
      unsigned map_lo = map_addr | (page == page_lo ? addr_lo & 0x1fff : 0x0000);
      unsigned map_hi = map_addr | (page == page_hi ? addr_hi & 0x1fff : 0x1fff);

      unsigned out_adjust, out_length, out_offset;
      if(mode == MapMode::Direct) {
        out_offset = 0;
        out_length = map_hi - map_lo + 1;
      } else if(mode == MapMode::Linear) {
        out_offset = base + mirror(offset, length) - map_lo;
        out_length = min(map_hi - map_lo + 1, length - offset);
        offset += map_hi - map_lo + 1;
        offset %= length;
      } else if(mode == MapMode::Shadow) {
        out_offset = base + mirror(map_addr, length) - map_lo;
        out_length = map_hi - map_lo + 1;
      }

      if(rd) rdpage[map_page].append({ rd, map_lo, out_length, out_offset });
      if(wr) wrpage[map_page].append({ wr, map_lo, out_length, out_offset });
    }
  }
}

bool Bus::load_cart() {
  if(cartridge.loaded() == true) return false;

  map_reset();
  map_xml();
  map_system();
  return true;
}

void Bus::unload_cart() {
}

void Bus::map_reset() {
  for(unsigned n = 0; n < 2048; n++) {
    rdpage[n].reset();
    wrpage[n].reset();
  }
}

void Bus::map_xml() {
  foreach(m, cartridge.mapping) {
    map(m.mode, m.banklo, m.bankhi, m.addrlo, m.addrhi, m.read, m.write, m.offset, m.size);
  }
}

void Bus::map_system() {
  map(
    MapMode::Linear, 0x00, 0x3f, 0x0000, 0x1fff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram },
    0x000000, 0x002000
  );
  map(
    MapMode::Linear, 0x80, 0xbf, 0x0000, 0x1fff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram },
    0x000000, 0x002000
  );
  map(
    MapMode::Linear, 0x7e, 0x7f, 0x0000, 0xffff,
    { &StaticRAM::read, &memory::wram }, { &StaticRAM::write, &memory::wram }
  );
}

void Bus::power() {
  foreach(n, memory::wram) n = config.cpu.wram_init_value;
  reset();
}

void Bus::reset() {
  map(MapMode::Direct, 0x00, 0x3f, 0x2100, 0x213f, { &PPU::mmio_read, &ppu }, { &PPU::mmio_write, &ppu });
  map(MapMode::Direct, 0x80, 0xbf, 0x2100, 0x213f, { &PPU::mmio_read, &ppu }, { &PPU::mmio_write, &ppu });

  map(MapMode::Direct, 0x00, 0x3f, 0x2140, 0x2183, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  map(MapMode::Direct, 0x80, 0xbf, 0x2140, 0x2183, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  map(MapMode::Direct, 0x00, 0x3f, 0x4016, 0x4017, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  map(MapMode::Direct, 0x80, 0xbf, 0x4016, 0x4017, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  map(MapMode::Direct, 0x00, 0x3f, 0x4200, 0x421f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  map(MapMode::Direct, 0x80, 0xbf, 0x4200, 0x421f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });

  map(MapMode::Direct, 0x00, 0x3f, 0x4300, 0x437f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
  map(MapMode::Direct, 0x80, 0xbf, 0x4300, 0x437f, { &CPU::mmio_read, &cpu }, { &CPU::mmio_write, &cpu });
}

}
