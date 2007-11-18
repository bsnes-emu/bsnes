#include "../../base.h"

#include "dsp1emu.cpp"

void DSP1::init() {}
void DSP1::enable() {}

void DSP1::power() {
  reset();
}

void DSP1::reset() {
  dsp1.reset();
}

/*****
 * addr_decode()
 * determine whether address is accessing
 * data register (DR) or status register (SR)
 * -- 0 (false) = DR
 * -- 1 (true ) = SR
 *
 * note: there is no need to bounds check addresses,
 * as memory mapper will not allow DSP1 accesses outside
 * of expected ranges
 *****/
bool DSP1::addr_decode(uint16 addr) {
  switch(cartridge.info.dsp1_mapper) {
    case Cartridge::DSP1LoROM1MB: {
    //$[20-3f]:[8000-bfff] = DR, $[20-3f]:[c000-ffff] = SR
      return (addr >= 0xc000);
    }

    case Cartridge::DSP1LoROM2MB: {
    //$[60-6f]:[0000-3fff] = DR, $[60-6f]:[4000-7fff] = SR
      return (addr >= 0x4000);
    }

    case Cartridge::DSP1HiROM: {
    //$[00-1f]:[6000-6fff] = DR, $[00-1f]:[7000-7fff] = SR
      return (addr >= 0x7000);
    }
  }

  return 0;
}

uint8 DSP1::read(uint addr) {
  return (addr_decode(addr) == 0) ? dsp1.getDr() : dsp1.getSr();
}

void DSP1::write(uint addr, uint8 data) {
  if(addr_decode(addr) == 0) {
    dsp1.setDr(data);
  }
}
