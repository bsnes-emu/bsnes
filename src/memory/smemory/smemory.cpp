#include <../base.hpp>
#include <../chip/chip.hpp>
#include <../cart/cart.hpp>
#define SMEMORY_CPP

#include "mapper/system.cpp"
#include "mapper/generic.cpp"
#include "mapper/chip.cpp"

void sBus::power() {
  for(unsigned i = 0x2000; i <= 0x5fff; i++) memory::mmio.map(i, memory::mmio_unmapped);
  for(unsigned i = 0; i < memory::wram.size(); i++) memory::wram[i] = snes.config.cpu.wram_init_value;
  reset();
}

void sBus::reset() {
}

bool sBus::load_cart() {
  if(cartridge.loaded() == true) return false;

  map_reset();
  map_generic();
  map_system();

  if(cartridge.has_cx4())   map_cx4();
  if(cartridge.has_dsp1())  map_dsp1();
  if(cartridge.has_dsp2())  map_dsp2();
  if(cartridge.has_dsp3())  map_dsp3();
  if(cartridge.has_dsp4())  map_dsp4();
  if(cartridge.has_obc1())  map_obc1();
  if(cartridge.has_st010()) map_st010();

  return true;
}

void sBus::unload_cart() {
}

sBus::sBus() {
}

sBus::~sBus() {
}
