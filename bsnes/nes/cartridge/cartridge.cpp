#include <nes/nes.hpp>

namespace NES {

Cartridge cartridge;

void Cartridge::Main() {
  cartridge.main();
}

void Cartridge::main() {
  mapper->main();
}

void Cartridge::load(const string &xml, const uint8_t *data, unsigned size) {
  rom_size = size - 16;
  rom_data = new uint8[rom_size];
  memcpy(rom_data, data + 16, size - 16);

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

  mirroring = ((data[6] & 0x08) >> 2) | (data[6] & 0x01);

  uint8 mapperNumber = ((data[7] >> 4) << 4) | (data[6] >> 4);
  switch(mapperNumber) {
  default : mapper = &Mapper::none; break;
  case   1: mapper = &Mapper::mmc1; break;
  case   2: mapper = &Mapper::uorom; break;
  case   3: mapper = &Mapper::cnrom; break;
  case   4: mapper = &Mapper::mmc3; break;
  case   7: mapper = &Mapper::aorom; break;
  case  16: mapper = &Mapper::bandaiFCG; break;
  case  24: mapper = &Mapper::vrc6; Mapper::vrc6.abus_swap = 0; break;
  case  26: mapper = &Mapper::vrc6; Mapper::vrc6.abus_swap = 1; break;
  }

  system.load();
  loaded = true;
  sha256 = nall::sha256(rom_data, rom_size);
}

void Cartridge::unload() {
  if(loaded == false) return;

  delete[] rom_data;
  delete[] prg_data;
  delete[] chr_data;

  loaded = false;
}

unsigned Cartridge::ram_size() {
  return mapper->ram_size();
}

uint8* Cartridge::ram_data() {
  return mapper->ram_data();
}

void Cartridge::power() {
  create(Cartridge::Main, 21477272);
  mapper->power();
}

void Cartridge::reset() {
  create(Cartridge::Main, 21477272);
  mapper->reset();
}

Cartridge::Cartridge() {
  loaded = false;
}

uint8 Cartridge::prg_read(unsigned addr) {
  return mapper->prg_read(addr);
}

void Cartridge::prg_write(unsigned addr, uint8 data) {
  return mapper->prg_write(addr, data);
}

uint8 Cartridge::chr_read(unsigned addr) {
  return mapper->chr_read(addr);
}

void Cartridge::chr_write(unsigned addr, uint8 data) {
  return mapper->chr_write(addr, data);
}

uint8 Cartridge::ciram_read(unsigned addr) {
  return mapper->ciram_read(addr);
}

void Cartridge::ciram_write(unsigned addr, uint8 data) {
  return mapper->ciram_write(addr, data);
}

void Cartridge::serialize(serializer &s) {
  if(chr_ram) s.array(chr_data, chr_size);

  return mapper->serialize(s);
}

}
