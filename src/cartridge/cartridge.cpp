#include <../base.hpp>

#define CART_CPP
namespace SNES {

#include "cartridge_header.cpp"
#include "cartridge_loader.cpp"

namespace memory {
  MappedRAM cartrom, cartram, cartrtc;
  MappedRAM bscram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
  MappedRAM dmgrom, dmgram, dmgrtc;
};

Cartridge cartridge;

void Cartridge::load_begin(Mode cartridge_mode) {
  memory::cartrom.map(0, 0);
  memory::cartram.map(0, 0);
  memory::cartrtc.map(0, 0);
  memory::bscram.map (0, 0);
  memory::stArom.map (0, 0);
  memory::stAram.map (0, 0);
  memory::stBrom.map (0, 0);
  memory::stBram.map (0, 0);
  memory::dmgrom.map (0, 0);
  memory::dmgram.map (0, 0);
  memory::dmgrtc.map (0, 0);

  set(loaded,           false);
  set(bsx_flash_loaded, false);
  set(patched,          false);
  set(mode,             cartridge_mode);
}

void Cartridge::load_end() {
  memory::cartrom.write_protect(true);
  memory::cartram.write_protect(false);
  memory::cartrtc.write_protect(false);
  memory::bscram.write_protect (true);
  memory::stArom.write_protect (true);
  memory::stAram.write_protect (false);
  memory::stBrom.write_protect (true);
  memory::stBram.write_protect (false);
  memory::dmgrom.write_protect (true);
  memory::dmgram.write_protect (false);
  memory::dmgrtc.write_protect (false);

  bus.load_cart();
  set(loaded, true);
}

void Cartridge::unload() {
  if(loaded() == false) return;
  bus.unload_cart();

  memory::cartrom.reset();
  memory::cartram.reset();
  memory::cartrtc.reset();
  memory::bscram.reset();
  memory::stArom.reset();
  memory::stAram.reset();
  memory::stBrom.reset();
  memory::stBram.reset();
  memory::dmgrom.reset();
  memory::dmgram.reset();
  memory::dmgrtc.reset();

  set(loaded, false);
}

Cartridge::Cartridge() {
  set(loaded, false);
}

Cartridge::~Cartridge() {
  if(loaded() == true) unload();
}

void Cartridge::set_cartinfo(const Cartridge::cartinfo_t &source) {
  set(region,         source.region);
  set(mapper,         source.mapper);
  set(dsp1_mapper,    source.dsp1_mapper);

  set(has_bsx_slot,   source.bsx_slot);
  set(has_superfx,    source.superfx);
  set(has_sa1,        source.sa1);
  set(has_srtc,       source.srtc);
  set(has_sdd1,       source.sdd1);
  set(has_spc7110,    source.spc7110);
  set(has_spc7110rtc, source.spc7110rtc);
  set(has_cx4,        source.cx4);
  set(has_dsp1,       source.dsp1);
  set(has_dsp2,       source.dsp2);
  set(has_dsp3,       source.dsp3);
  set(has_dsp4,       source.dsp4);
  set(has_obc1,       source.obc1);
  set(has_st010,      source.st010);
  set(has_st011,      source.st011);
  set(has_st018,      source.st018);
}

//==========
//cartinfo_t
//==========

void Cartridge::cartinfo_t::reset() {
  type        = TypeUnknown;
  mapper      = LoROM;
  dsp1_mapper = DSP1Unmapped;
  region      = NTSC;

  rom_size = 0;
  ram_size = 0;

  bsx_slot   = false;
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

Cartridge::cartinfo_t::cartinfo_t() {
  reset();
}

};
