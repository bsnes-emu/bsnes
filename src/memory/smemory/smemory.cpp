#include "../../base.h"
#define SMEMORY_CPP

#include "mapper/system.cpp"
#include "mapper/generic.cpp"
#include "mapper/chip.cpp"

void sBus::power() {
  for(unsigned i = 0x2000; i <= 0x5fff; i++) memory::mmio.map(i, memory::mmio_unmapped);
  for(unsigned i = 0; i < memory::wram.size(); i++) memory::wram[i] = config::cpu.wram_init_value;
  reset();
}

void sBus::reset() {
  set_speed(false);
}

void sBus::load_cart() {
  if(is_cart_loaded == true) return;

  map_reset();
  map_generic();
  map_system();

  if(cartridge.info.cx4)   map_cx4();
  if(cartridge.info.dsp1)  map_dsp1();
  if(cartridge.info.dsp2)  map_dsp2();
  if(cartridge.info.dsp3)  map_dsp3();
  if(cartridge.info.dsp4)  map_dsp4();
  if(cartridge.info.obc1)  map_obc1();
  if(cartridge.info.st010) map_st010();

  snes.set_region(cartridge.region() == Cartridge::NTSC ? SNES::NTSC : SNES::PAL);

  is_cart_loaded = true;
}

void sBus::unload_cart() {
  if(is_cart_loaded == false) return;

  is_cart_loaded = false;
}

bool sBus::cart_loaded() {
  return is_cart_loaded;
}

sBus::sBus() {
  is_cart_loaded = false;
}

sBus::~sBus() {
  unload_cart();
}
