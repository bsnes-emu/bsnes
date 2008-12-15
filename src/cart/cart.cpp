#include <../base.hpp>
#define CART_CPP

#include <nall/crc32.hpp>
#include <nall/ups.hpp>

#include "cart_load.cpp"
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

const char* Cartridge::name() { return info.filename; }
Cartridge::CartridgeMode Cartridge::mode() { return info.mode; }
Cartridge::MemoryMapper Cartridge::mapper() { return info.mapper; }
Cartridge::Region Cartridge::region() { return info.region; }
bool Cartridge::loaded() { return cart.loaded; }

void Cartridge::load_begin(CartridgeMode mode) {
  cart.rom = cart.ram = cart.rtc = 0;
  bs.ram   = 0;
  stA.rom  = stA.ram  = 0;
  stB.rom  = stB.ram  = 0;

  cart.rom_size = cart.ram_size = cart.rtc_size = 0;
  bs.ram_size   = 0;
  stA.rom_size  = stA.ram_size  = 0;
  stB.rom_size  = stB.ram_size  = 0;

  info.mode = mode;
  info.patched = false;

  info.bsxcart  = false;
  info.bsxflash = false;
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

  if(file::exists(get_cheat_filename(cart.fn, "cht"))) {
    cheat.clear();
    cheat.load(cheatfn);
  }

  cart.loaded = true;
  bus.load_cart();
}

bool Cartridge::unload() {
  if(cart.loaded == false) return false;

  bus.unload_cart();

  switch(info.mode) {
    case ModeNormal:      unload_cart_normal(); break;
    case ModeBSX:         unload_cart_bsx();    break;
    case ModeBSC:         unload_cart_bsc();    break;
    case ModeSufamiTurbo: unload_cart_st();     break;
  }

  if(cart.rom) { delete[] cart.rom; cart.rom = 0; }
  if(cart.ram) { delete[] cart.ram; cart.ram = 0; }
  if(cart.rtc) { delete[] cart.rtc; cart.rtc = 0; }
  if(bs.ram)   { delete[] bs.ram;   bs.ram   = 0; }
  if(stA.rom)  { delete[] stA.rom;  stA.rom  = 0; }
  if(stA.ram)  { delete[] stA.ram;  stA.ram  = 0; }
  if(stB.rom)  { delete[] stB.rom;  stB.rom  = 0; }
  if(stB.ram)  { delete[] stB.ram;  stB.ram  = 0; }

  if(cheat.count() > 0 || file::exists(get_cheat_filename(cart.fn, "cht"))) {
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

//

void Cartridge::cartinfo_t::reset() {
  type        = TypeUnknown;
  mapper      = LoROM;
  dsp1_mapper = DSP1Unmapped;
  region      = NTSC;

  rom_size = 0;
  ram_size = 0;

  bsxslot    = false;
  superfx    = false;
  sa1        = false;
  srtc       = false;
  sdd1       = false;
  spc7110    = false;
  spc7110rtc = false;
  cx4        = false;
  dsp1       = false;
  dsp2       = false;
  dsp3       = false;
  dsp4       = false;
  obc1       = false;
  st010      = false;
  st011      = false;
  st018      = false;
}

//apply cart-specific settings to current cartridge mode settings
Cartridge::info_t& Cartridge::info_t::operator=(const Cartridge::cartinfo_t &source) {
  mapper      = source.mapper;
  dsp1_mapper = source.dsp1_mapper;
  region      = source.region;

  bsxslot    = source.bsxslot;
  superfx    = source.superfx;
  sa1        = source.sa1;
  srtc       = source.srtc;
  sdd1       = source.sdd1;
  spc7110    = source.spc7110;
  spc7110rtc = source.spc7110rtc;
  cx4        = source.cx4;
  dsp1       = source.dsp1;
  dsp2       = source.dsp2;
  dsp3       = source.dsp3;
  dsp4       = source.dsp4;
  obc1       = source.obc1;
  st010      = source.st010;
  st011      = source.st011;
  st018      = source.st018;

  return *this;
}
