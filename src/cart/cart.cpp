#include "../base.h"

#include "cart_normal.cpp"
#include "cart_bsx.cpp"
#include "cart_bsc.cpp"
#include "cart_st.cpp"

#include "cart_file.cpp"
#include "cart_header.cpp"

namespace memory {
  MappedRAM cartrom, cartram;
  MappedRAM bscram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
};

Cartridge cartridge;

Cartridge::MemoryMapper Cartridge::mapper() { return info.mapper; }
Cartridge::Region Cartridge::region() { return info.region; }

bool Cartridge::loaded() { return cart.loaded; }

void Cartridge::load_begin(CartridgeType cart_type) {
  cart.rom = cart.ram = 0;
  bs.ram   = 0;
  stA.rom  = stA.ram  = 0;
  stB.rom  = stB.ram  = 0;

  cart.rom_size = cart.ram_size = 0;
  bs.ram_size   = 0;
  stA.rom_size  = stA.ram_size  = 0;
  stB.rom_size  = stB.ram_size  = 0;

  info.type = cart_type;

  info.bsxbase  = false;
  info.bsxcart  = false;
  info.bsxflash = false;
  info.st       = false;

  info.superfx = false;
  info.sa1     = false;
  info.srtc    = false;
  info.sdd1    = false;
  info.cx4     = false;
  info.dsp1    = false;
  info.dsp2    = false;
  info.dsp3    = false;
  info.dsp4    = false;
  info.obc1    = false;
  info.st010   = false;
  info.st011   = false;
  info.st018   = false;

  info.dsp1_mapper = DSP1Unmapped;

  info.header_index = 0xffc0;
  info.mapper = LoROM;
  info.name[0] = 0;
  info.region = NTSC;

  info.rom_size = 0;
  info.ram_size = 0;
}

void Cartridge::load_end() {
  memory::cartrom.map(cart.rom, cart.rom_size);
  memory::cartram.map(cart.ram, cart.ram_size);
  memory::bscram.map(bs.ram, bs.ram_size);
  memory::stArom.map(stA.rom, stA.rom_size);
  memory::stAram.map(stA.ram, stA.ram_size);
  memory::stBrom.map(stB.rom, stB.rom_size);
  memory::stBram.map(stB.ram, stB.ram_size);

  memory::cartrom.write_protect(true);
  memory::cartram.write_protect(false);
  memory::bscram.write_protect(true);
  memory::stArom.write_protect(true);
  memory::stAram.write_protect(false);
  memory::stBrom.write_protect(true);
  memory::stBram.write_protect(false);

  cart.loaded = true;
  bus.load_cart();
}

bool Cartridge::unload() {
  if(cart.loaded == false) return false;

  bus.unload_cart();

  switch(info.type) {
    case CartridgeNormal: unload_cart_normal(); break;
    case CartridgeBSX: unload_cart_bsx(); break;
    case CartridgeBSC: unload_cart_bsc(); break;
    case CartridgeSufamiTurbo: unload_cart_st(); break;
  }

  safe_free(cart.rom);
  safe_free(cart.ram);
  safe_free(bs.ram);
  safe_free(stA.rom);
  safe_free(stA.ram);
  safe_free(stB.rom);
  safe_free(stB.ram);

  cart.loaded = false;
  return true;
}

Cartridge::Cartridge() {
  cart.loaded = false;
}

Cartridge::~Cartridge() {
  if(cart.loaded == true) unload();
}
