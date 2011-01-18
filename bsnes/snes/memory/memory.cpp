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
  unsigned id = idcount++;
  assert(id < 255);
  reader[id] = rd;
  writer[id] = wr;

  if(length == 0) length = (bank_hi - bank_lo + 1) * (addr_hi - addr_lo + 1);

  unsigned offset = 0;
  for(unsigned bank = bank_lo; bank <= bank_hi; bank++) {
    for(unsigned addr = addr_lo; addr <= addr_hi; addr++) {
      unsigned destaddr = (bank << 16) | addr;
      if(mode == MapMode::Linear) {
        destaddr = base + mirror(offset, length);
        offset = (offset + 1) % length;
      } else if(mode == MapMode::Shadow) {
        destaddr = base + mirror(destaddr, length);
      }
      lookup[(bank << 16) | addr] = id;
      target[(bank << 16) | addr] = destaddr;
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
  function<uint8 (unsigned)> reader = [](unsigned) { return cpu.regs.mdr; };
  function<void (unsigned, uint8)> writer = [](unsigned, uint8) {};

  idcount = 0;
  map(MapMode::Direct, 0x00, 0xff, 0x0000, 0xffff, reader, writer);
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

Bus::Bus() {
  lookup = new uint8 [16 * 1024 * 1024];
  target = new uint32[16 * 1024 * 1024];
}

Bus::~Bus() {
  delete[] lookup;
  delete[] target;
}

}
