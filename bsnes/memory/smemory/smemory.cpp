#include <../base.hpp>

#define SMEMORY_CPP
namespace SNES {

sBus bus;

#include "serialization.cpp"

void sBus::power() {
  foreach(n, memory::wram) n = config.cpu.wram_init_value;
}

void sBus::reset() {
}

bool sBus::load_cart() {
  if(cartridge.loaded() == true) return false;

  map_reset();
  map_xml();
  map_system();
  return true;
}

void sBus::unload_cart() {
}

void sBus::map_reset() {
  map(MapMode::Direct, 0x00, 0xff, 0x0000, 0xffff, memory::memory_unmapped);
  map(MapMode::Direct, 0x00, 0x3f, 0x2000, 0x5fff, memory::mmio);
  map(MapMode::Direct, 0x80, 0xbf, 0x2000, 0x5fff, memory::mmio);
  for(unsigned i = 0x2000; i <= 0x5fff; i++) memory::mmio.map(i, memory::mmio_unmapped);
}

void sBus::map_xml() {
  foreach(m, cartridge.mapping) {
    if(m.memory) {
      map(m.mode, m.banklo, m.bankhi, m.addrlo, m.addrhi, *m.memory, m.offset, m.size);
    } else if(m.mmio) {
      for(unsigned i = m.addrlo; i <= m.addrhi; i++) memory::mmio.map(i, *m.mmio);
    }
  }
}

void sBus::map_system() {
  map(MapMode::Linear, 0x00, 0x3f, 0x0000, 0x1fff, memory::wram, 0x000000, 0x002000);
  map(MapMode::Linear, 0x80, 0xbf, 0x0000, 0x1fff, memory::wram, 0x000000, 0x002000);
  map(MapMode::Linear, 0x7e, 0x7f, 0x0000, 0xffff, memory::wram);
}

sBus::sBus() {
}

sBus::~sBus() {
}

};
