#include <gba/gba.hpp>

namespace GBA {

Cartridge cartridge;

bool Cartridge::load(const string &markup, const uint8_t *data, unsigned size) {
  if(cartridge.rom.data) delete[] cartridge.rom.data;
  cartridge.rom.data = new uint8[cartridge.rom.size = size];
  memcpy(cartridge.rom.data, data, size);

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
