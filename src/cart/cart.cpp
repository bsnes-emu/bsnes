#include "../base.h"
#define CART_CPP

#include <nall/crc32.hpp>
#include <nall/ups.hpp>

#include "cart_normal.cpp"
#include "cart_bsx.cpp"
#include "cart_bsc.cpp"
#include "cart_st.cpp"

#include "cart_file.cpp"
#include "cart_header.cpp"

namespace memory {
  MappedRAM cartrom, cartram, cartrtc;
  MappedRAM bscram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
};

Cartridge cartridge;

Cartridge::MemoryMapper Cartridge::mapper() { return info.mapper; }
Cartridge::Region Cartridge::region() { return info.region; }

bool Cartridge::loaded() { return cart.loaded; }

void Cartridge::load_begin(CartridgeType cart_type) {
  cart.rom = cart.ram = cart.rtc = 0;
  bs.ram   = 0;
  stA.rom  = stA.ram  = 0;
  stB.rom  = stB.ram  = 0;

  cart.rom_size = cart.ram_size = cart.rtc_size = 0;
  bs.ram_size   = 0;
  stA.rom_size  = stA.ram_size  = 0;
  stB.rom_size  = stB.ram_size  = 0;

  info.type = cart_type;
  info.patched = false;

  info.bsxbase  = false;
  info.bsxcart  = false;
  info.bsxflash = false;
  info.st       = false;

  info.superfx    = false;
  info.sa1        = false;
  info.srtc       = false;
  info.sdd1       = false;
  info.spc7110    = false;
  info.spc7110rtc = false;
  info.cx4        = false;
  info.dsp1       = false;
  info.dsp2       = false;
  info.dsp3       = false;
  info.dsp4       = false;
  info.obc1       = false;
  info.st010      = false;
  info.st011      = false;
  info.st018      = false;

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
  memory::cartrtc.map(cart.rtc, cart.rtc_size);
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

  if(fexists(get_cheat_filename(cart.fn, "cht"))) {
    cheat.clear();
    cheat.load(cheatfn);
  }

  cart.loaded = true;
  bus.load_cart();
}

bool Cartridge::unload() {
  if(cart.loaded == false) return false;

  bus.unload_cart();

  switch(info.type) {
    case CartridgeNormal:      unload_cart_normal(); break;
    case CartridgeBSX:         unload_cart_bsx();    break;
    case CartridgeBSC:         unload_cart_bsc();    break;
    case CartridgeSufamiTurbo: unload_cart_st();     break;
  }

  if(cart.rom) { delete[] cart.rom; cart.rom = 0; }
  if(cart.ram) { delete[] cart.ram; cart.ram = 0; }
  if(cart.rtc) { delete[] cart.rtc; cart.rtc = 0; }
  if(bs.ram)   { delete[] bs.ram;   bs.ram   = 0; }
  if(stA.rom)  { delete[] stA.rom;  stA.rom  = 0; }
  if(stA.ram)  { delete[] stA.ram;  stA.ram  = 0; }
  if(stB.rom)  { delete[] stB.rom;  stB.rom  = 0; }
  if(stB.ram)  { delete[] stB.ram;  stB.ram  = 0; }

  char fn[PATH_MAX];
  strcpy(fn, cart.fn);
  modify_extension(fn, "cht");
  if(cheat.count() > 0 || fexists(get_cheat_filename(cart.fn, "cht"))) {
    cheat.save(cheatfn);
    cheat.clear();
  }

  cart.loaded = false;
  return true;
}

Cartridge::Cartridge() {
  cart.loaded = false;
}

Cartridge::~Cartridge() {
  if(cart.loaded == true) unload();
}
