#include <gba/gba.hpp>

namespace GBA {

#include "eeprom.cpp"
Cartridge cartridge;

bool Cartridge::load(const string &markup, const uint8_t *data, unsigned size) {
  if(rom.data) delete[] rom.data;
  rom.data = new uint8[rom.size = 32 * 1024 * 1024];
  for(unsigned addr = 0; addr < 32 * 1024 * 1024; addr++) {
    rom.data[addr] = data[Bus::mirror(addr, size)];
  }

  if(ram.data) delete[] ram.data;
  ram.data = new uint8[ram.size = 64 * 1024]();

  sha256 = nall::sha256(rom.data, rom.size);

  return loaded = true;
}

void Cartridge::unload() {
  if(loaded) return;
  loaded = false;

  delete[] rom.data;
  rom.data = nullptr;
  rom.size = 0u;
}

void Cartridge::power() {
  eeprom.power();

  has_eeprom = false;
}

uint32 Cartridge::read(uint32 addr, uint32 size) {
  if(has_eeprom && (addr & 0x0f000000) == 0x0d000000) return eeprom.read();

  if((addr & 0x0e000000) == 0x08000000) return rom.read(addr & 0x01ffffff, size);
  if((addr & 0x0e000000) == 0x0a000000) return rom.read(addr & 0x01ffffff, size);
  if((addr & 0x0e000000) == 0x0c000000) return rom.read(addr & 0x01ffffff, size);
  if((addr & 0x0e000000) == 0x0e000000) return ram.read(addr & 0x0000ffff, size);
}

void Cartridge::write(uint32 addr, uint32 size, uint32 word) {
  if((addr & 0x0f000000) == 0x0d000000) { has_eeprom = true; return eeprom.write(word & 1); }

  if((addr & 0x0e000000) == 0x0e000000) return ram.write(addr & 0x0000ffff, size, word);
}

Cartridge::Cartridge() {
  loaded = false;
}

}
