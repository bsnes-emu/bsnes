#include <../base.hpp>

#define CARTRIDGE_CPP
namespace SNES {

#include "header.cpp"

namespace memory {
  MappedRAM cartrom, cartram, cartrtc;
  MappedRAM bsxflash, bsxram, bsxpram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
  MappedRAM gbrom, gbram;
};

Cartridge cartridge;

void Cartridge::load(Mode cartridge_mode) {
  cartinfo_t cartinfo;
  read_header(cartinfo, memory::cartrom.data(), memory::cartrom.size());
  set_cartinfo(cartinfo);

  set(mode, cartridge_mode);

  if(cartinfo.ram_size > 0) {
    memory::cartram.map(new(zeromemory) uint8_t[cartinfo.ram_size], cartinfo.ram_size);
  }

  if(cartinfo.srtc || cartinfo.spc7110rtc) {
    memory::cartrtc.map(new(zeromemory) uint8_t[20], 20);
  }

  if(mode() == ModeBsx) {
    memory::bsxram.map (new(zeromemory) uint8_t[ 32 * 1024],  32 * 1024);
    memory::bsxpram.map(new(zeromemory) uint8_t[512 * 1024], 512 * 1024);
  }

  if(mode() == ModeSufamiTurbo) {
    if(memory::stArom.data()) memory::stAram.map(new(zeromemory) uint8_t[128 * 1024], 128 * 1024);
    if(memory::stBrom.data()) memory::stBram.map(new(zeromemory) uint8_t[128 * 1024], 128 * 1024);
  }

  if(mode() == ModeSuperGameBoy) {
    if(memory::gbrom.data()) memory::gbram.map(new(zeromemory) uint8_t[64 * 1024], 64 * 1024);
  }

  memory::cartrom.write_protect(true);
  memory::cartram.write_protect(false);
  memory::cartrtc.write_protect(false);
  memory::bsxflash.write_protect(true);
  memory::bsxram.write_protect(false);
  memory::bsxpram.write_protect(false);
  memory::stArom.write_protect(true);
  memory::stAram.write_protect(false);
  memory::stBrom.write_protect(true);
  memory::stBram.write_protect(false);
  memory::gbrom.write_protect(true);
  memory::gbram.write_protect(false);

  bus.load_cart();
  set(loaded, true);
}

void Cartridge::unload() {
  memory::cartrom.reset();
  memory::cartram.reset();
  memory::cartrtc.reset();
  memory::bsxflash.reset();
  memory::bsxram.reset();
  memory::bsxpram.reset();
  memory::stArom.reset();
  memory::stAram.reset();
  memory::stBrom.reset();
  memory::stBram.reset();
  memory::gbrom.reset();
  memory::gbram.reset();

  if(loaded() == false) return;
  bus.unload_cart();
  set(loaded, false);
}

Cartridge::Type Cartridge::detect_image_type(uint8_t *data, unsigned size) const {
  cartinfo_t info;
  read_header(info, data, size);
  return info.type;
}

Cartridge::Cartridge() {
  set(loaded, false);
  unload();
}

Cartridge::~Cartridge() {
  unload();
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

