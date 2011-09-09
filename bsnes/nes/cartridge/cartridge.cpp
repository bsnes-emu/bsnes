#include <nes/nes.hpp>

namespace NES {

Cartridge cartridge;

void Cartridge::load(const string &xml, const uint8_t *data, unsigned size) {
  prg_size = data[4] * 0x4000;
  chr_size = data[5] * 0x2000;

  prg_data = new uint8[prg_size];
  memcpy(prg_data, data + 16, prg_size);

  if(chr_size) {
    chr_ram = false;
    chr_data = new uint8[chr_size];
    memcpy(chr_data, data + 16 + prg_size, chr_size);
  } else {
    chr_ram = true;
    chr_size = 0x2000;
    chr_data = new uint8[chr_size]();
  }

  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;

  delete[] prg_data;
  delete[] chr_data;

  loaded = false;
}

Cartridge::Cartridge() {
  loaded = false;
}

uint8 Cartridge::prg_read(uint16 addr) {
  if(addr >= 0x8000 && addr <= 0xffff) {
    addr &= 0x7fff;
    if(addr >= prg_size) addr &= prg_size - 1;
    return prg_data[addr];
  }
}

void Cartridge::prg_write(uint16 addr, uint8 data) {
}

uint8 Cartridge::chr_read(uint16 addr) {
  return chr_data[addr & 0x1fff];
}

void Cartridge::chr_write(uint16 addr, uint8 data) {
  if(chr_ram == false) return;
  chr_data[addr & 0x1fff] = data;
}

}
