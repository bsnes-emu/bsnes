#include <gba/gba.hpp>

#define CARTRIDGE_CPP
namespace GBA {

Cartridge cartridge;

void Cartridge::load(const string &markup, const uint8_t *data, unsigned size) {
  romdata = new uint8_t[romsize = size];
  memcpy(romdata, data, size);
  sha256 = nall::sha256(data, size);

  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;

  delete[] romdata;
  romdata = nullptr;
  romsize = 0u;

  loaded = false;
}

void Cartridge::power() {
}

Cartridge::Cartridge() {
  loaded = false;
  romdata = nullptr;
  romsize = 0u;
}

}
