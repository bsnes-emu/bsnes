#include <gba/gba.hpp>

namespace GBA {

Cartridge cartridge;

bool Cartridge::load(const string &markup, const uint8_t *data, unsigned size) {
  if(cartridge.rom.data) delete[] cartridge.rom.data;
  cartridge.rom.data = new uint8[cartridge.rom.size = 32 * 1024 * 1024];
  for(unsigned addr = 0; addr < 32 * 1024 * 1024; addr++) {
    cartridge.rom.data[addr] = data[Bus::mirror(addr, size)];
  }

  if(cartridge.ram.data) delete[] cartridge.ram.data;
  cartridge.ram.data = new uint8[cartridge.ram.size = 64 * 1024]();

  sha256 = nall::sha256(cartridge.rom.data, cartridge.rom.size);

  return loaded = true;
}

void Cartridge::unload() {
  if(loaded) return;
  loaded = false;

  delete[] cartridge.rom.data;
  cartridge.rom.data = nullptr;
  cartridge.rom.size = 0u;
}

Cartridge::Cartridge() {
  loaded = false;
}

}
