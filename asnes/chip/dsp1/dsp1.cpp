#include <snes.hpp>

#define DSP1_CPP
namespace SNES {

DSP1 dsp1;
DSP1DR dsp1dr;
DSP1SR dsp1sr;

#include "serialization.cpp"
#include "dsp1emu.cpp"

void DSP1::init() {
}

void DSP1::enable() {
}

void DSP1::power() {
  reset();
}

void DSP1::reset() {
  dsp1.reset();
}

uint8 DSP1DR::read(unsigned addr) { return dsp1.dsp1.getDr(); }
void DSP1DR::write(unsigned addr, uint8 data) { dsp1.dsp1.setDr(data); }

uint8 DSP1SR::read(unsigned addr) { return dsp1.dsp1.getSr(); }
void DSP1SR::write(unsigned addr, uint8 data) {}

}
